#include <string>
#include <cmath>

#include "ReadImage.cpp"
#include "WriteImage.cpp"

using namespace std;

/* function prototypes */
void allocate2DArrays(int** &array, int rows, int columns);
void generateMaskSizes(int* maskSizes, int initialSigma, int numberOfIntermediateLevels);
void generateNextLevel(int** dataIn, int** &dataOut, int& numRows, int& numColumns, int* dataOutSize, double maskSize);
void performNormalization(int** dataOut, int& numRows, int& numColumns, int& maxGreyValue);
string buildOutputFilename(string& filepath, int index, int& numLevels, int& numIntermediateLevels);


void generateGaussianPyramid(string pathToImage, int initialSigma, int numberOfLevels, int numberOfIntermediates)
{
  // variables
  int numberOfBins = 1 + numberOfLevels * numberOfIntermediates;
  int numberOfMasks = numberOfIntermediates + 1;
  int*** dataIn;
  int*** dataOut = new int**[numberOfBins];
  int** dataOutSizes = new int*[numberOfBins];
  int* maskSizes = new int[numberOfMasks];
  int numRows, numColumns, maxGreyValue;
  int parentIndex, maskIndex;
  string outputFilePath;

  // read image in
  ReadImage(pathToImage.c_str(), dataIn, numRows, numColumns, maxGreyValue);

  // allocate dataOutSizes with numberOfBins rows (one row for each level / intermediate level)
  // and with 2 columns (for storing x,y dimensions of each level)
  allocate2DArrays(dataOutSizes, 2, numberOfBins);

  // generate the mask sizes that will be used
  generateMaskSizes(maskSizes, initialSigma, numberOfIntermediates);

  // store the dimensions of the zero-th level in dataOutSizes
  dataOutSizes[0][0] = numRows;
  dataOutSizes[0][1] = numColumns;

  // store the zero-th level in dataOut
  dataOut[0] = *(dataIn);

  // loop once for each level (octave) in the pyramid
  for(int i = 1; i < numberOfBins; i++)
  {

    // generate indexes (I derived these algorithms on paper)
    parentIndex = 3 * ( (i - 1) / 3 );
    maskIndex = i - parentIndex - 1;

    // generate the next level / intermediate level of the pyramid
    generateNextLevel( dataOut[parentIndex], dataOut[i], dataOutSizes[parentIndex][0],
                       dataOutSizes[parentIndex][1], dataOutSizes[i], maskSizes[maskIndex] );

  }

  // normalize (this may not be required?)
  for(int i = 0; i < numberOfBins; i++)
  {
//    performNormalization(dataOut[i], dataOutSizes[i][0], dataOutSizes[i][1], maxGreyValue);
  }

  // write output images
  for(int i = 0; i < numberOfBins; i++)
  {
    outputFilePath = buildOutputFilename(pathToImage, i, numberOfLevels, numberOfIntermediates);

    WriteImage(outputFilePath.c_str(), dataOut[i], dataOutSizes[i][1], dataOutSizes[i][0], maxGreyValue);
  }
}


/*
 * Allocates space for 2D arrays, given an "array".
 *
 * Allocates space given the dimensions "rows" and "columns",
 * where the first dimension will be the number of columns
 * and the second dimension will be the number of rows in each column.
 */
void allocate2DArrays(int** &array, int rows, int columns)
{
  array = new int*[columns];

  for(int i = 0; i < columns; i++)
    array[i] = new int[rows];
}


/*
 * Generates the mask sizes that will be used to generate levels / intermediate levels.
 */
void generateMaskSizes(int* maskSizes, int initialSigma, int numberOfIntermediateLevels)
{
  // variables
  float k = pow( 2.0, 1.0 / numberOfIntermediateLevels);
  int maskSize = (int) ceil( k * numberOfIntermediateLevels);

  // store initialSigma as the last maskSize at maskSizes[numberOfIntermediateLevels]
  maskSizes[numberOfIntermediateLevels] = initialSigma;

  // loop from [0 , numberOfIntermediateLevels ) as i
  for(int i = 0; i < numberOfIntermediateLevels; i++ )
  {
    // maskSizes[i] = int(k) * (i + 1)
    maskSizes[i] = maskSize * (i + 1);
  }


}


/*
 * Generates the next level or intermediate level in the pyramid.
 *
 * "numRows" and "numColumns" should be the dimensions of "dataIn".
 *
 * The dimensions of "dataOut" will be stored inside of "dataOutSize",
 * after those dimensions are calculated.
 */
void generateNextLevel(int** dataIn, int** &dataOut, int& numRows, int& numColumns, int* dataOutSize, double maskSize)
{
  // variables
  double rows = ceil(numRows / maskSize);
  double columns = ceil(numColumns / maskSize);
  int currentX, startIndexX, endIndexX;
  int currentY, startIndexY, endIndexY;
  int sum;

  // allocate space for dataOut, which needs ceil( size / sigma ) space
  allocate2DArrays(dataOut, (int) rows, (int) columns );

  // store the calculated dimensions for dataOut into dataOutSize
  dataOutSize[0] = (int) rows;
  dataOutSize[1] = (int) columns;

  // loop through all the data in previousOctave, increment through data with maskSize
  for(int x = 0; x < numRows; x += maskSize)
  {
    for(int y = 0; y < numColumns; y += maskSize)
    {

      // generate start and end indexes
      startIndexX = x;
      endIndexX = x + maskSize;
      startIndexY = y;
      endIndexY = y + maskSize;

      // set sum to 0
      sum = 0;

      // loop from start to end index
      for(int indexX = startIndexX; indexX < endIndexX; indexX++)
      {
        for(int indexY = startIndexY; indexY < endIndexY; indexY++)
        {

          // check if the current index is a valid index (meaning the index is actually inside the image)
          if( (indexX < numRows) && (indexY < numColumns) )
          {
            // increment sum with value at dataIn[indexY][indexX]
            sum += dataIn[indexY][indexX];
          }

        }
      }

      // scale down the sum
      sum /= maskSize * maskSize;

      // set dataOut[startIndex/2][startIndex/2] = sum
      dataOut[startIndexY / int(maskSize)][startIndexX / int(maskSize)] = sum;
    }
  }

}


/*
 * Performs normalization across the data that will be written to file. This ensures that
 * every pixel value falls in the acceptable range.
 */
void performNormalization(int** dataOut, int& numRows, int& numColumns, int& maxGreyValue)
{
//  int denominator = maxGreyValue - minimumValue;
  int value;
  int maximumValue = maxGreyValue, minimumValue = maxGreyValue;
  double range = maximumValue - minimumValue;

  // loop through all pixels, finding lowest and highest value
  for(int x = 0; x < numRows; x++)
  {
    for(int y = 0; y < numColumns; y++)
    {

      if( dataOut[y][x] > maximumValue)
        maximumValue = dataOut[y][x];

      if( dataOut[y][x] < minimumValue && dataOut[y][x] >= 0)
        minimumValue = dataOut[y][x];
      value = dataOut[y][x];

    }
  }

  // loop through all pixels
  for(int x = 0; x < numRows; x++)
  {
    for(int y = 0; y < numColumns; y++)
    {

      value = dataOut[y][x];

      // apply mathematical transformation to each pixel value
      dataOut[y][x] = ( (value - minimumValue) * (maximumValue - 0) / range ) + 0;

    }
  }

}


/*
 * Builds the output filename.
 */
string buildOutputFilename(string& filepath, int index, int& numLevels, int& numIntermediateLevels)
{
  string outputString = "";
  int i = 0;
  int strlength = filepath.length();
  int level = index / numLevels;
  int intermediate = index % (numIntermediateLevels + 1);

  // copy the entire string until a "." is reached
  while( filepath[i] != '.')
  {
    outputString += filepath[i];
    i++;
  }

  // append level to output file string
  outputString += "_level=" + to_string(level);

  // intermediate level to output file string if > than 0
  if( intermediate > 0)
  {
    outputString += "_intermediate=" + to_string(intermediate);
  }

  // finish copying filepath into the output string
  for(i; i < strlength; i++)
  {
    outputString += filepath[i];
  }

  return outputString;
}












