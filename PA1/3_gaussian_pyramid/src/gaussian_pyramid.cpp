#include <string>
#include <cmath>

//todo remove this
#include <iostream>

#include "ReadImage.cpp"
#include "WriteImage.cpp"

using namespace std;

/* function prototypes */
void allocate2DArrays(int** &array, int rows, int columns);
void generateMaskSizes(int* maskSizes, int initialSigma, int numberOfIntermediateLevels);
void generateNextLevel(int** dataIn, int** &dataOut, int& numRows, int& numColumns, int* dataOutSize, double maskSize);



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

  cout << "about to read in the image" << endl;
  cout << "image path: " << pathToImage << endl;
  cout << "dataIn: " << dataIn << endl;

  // read image in
//  ReadImage(pathToImage.c_str(), dataIn, numRows, numColumns, maxGreyValue);

  ReadImage(pathToImage.c_str(), dataIn, numRows, numColumns, maxGreyValue);

  cout << "about to allocate space for dataOutSizes" << endl;

  // allocate dataOutSizes with numberOfBins rows (one row for each level / intermediate level)
  // and with 2 columns (for storing x,y dimensions of each level)
  allocate2DArrays(dataOutSizes, numberOfBins, 2);

  cout << "about to generate the mask sizes" << endl;

  // generate the mask sizes that will be used
  generateMaskSizes(maskSizes, initialSigma, numberOfIntermediates);

  cout << "about to store dimensions of zero-th level" << endl;

  // store the dimensions of the zero-th level in dataOutSizes
  dataOutSizes[0][0] = numRows;
  dataOutSizes[0][1] = numColumns;

  cout << "about to store zero-th level in dataOut" << endl;

  // store the zero-th level in dataOut
  dataOut[0] = *(dataIn);

  cout << "about to look through and create each level / intermediate level" << endl;

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


  cout << "about to write images to file" << endl;

  // write output images

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
  float k = pow( 2, 1 / numberOfIntermediateLevels);

  // store initialSigma as the last maskSize at maskSizes[numberOfIntermediateLevels]
  maskSizes[numberOfIntermediateLevels] = initialSigma;

  // loop from [0 , numberOfIntermediateLevels ) as i
  for(int i = 0; i < numberOfIntermediateLevels; i++ )
  {
    // maskSizes[i] = int(k) * (i + 1)
    maskSizes[i] = int(k) * (i + 1);
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
      endIndexX = x + maskSize - 1;
      startIndexY = y;
      endIndexY = y + maskSize - 1;

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

      // set dataOut[startIndex/2][startIndex/2] = sum
      dataOut[startIndexY / 2][startIndexX / 2] = sum;
    }
  }

}















