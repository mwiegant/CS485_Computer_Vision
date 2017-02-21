#include <fstream>

#include "Gauss_masks.c"
#include "ReadImage.cpp"
#include "WriteImage.cpp"

using namespace std;

namespace Gauss_2D_2Mask {

  /* function prototypes */
  void performHorizontalSmoothing(int** dataIn, int** dataOut, int& numRows, int& numColumns, int maskSize, float* mask);
  void performVerticalSmoothing(int** dataIn, int** dataOut, int& numRows, int& numColumns, int maskSize, float* mask);
  void performNormalization(int** dataOut, int& numRows, int& numColumns, int& maxGreyValue);
  string buildOutputFilename(string& filepath, int& sigma);


  /*
   * Performs 2D Gaussian Smoothing by calling helper functions to perform
   * the various steps required.
   *
   * 2D Gaussian Smoothing is performed by using 2 1D-masks.
   *
   * If any step fails, an error is returned.
  */
  void perform2D_Gaussian_Smoothing_2_masks(int sigma, string pathToImage)
  {
    int returnStatus = 0;
    int maskSize = 5 * sigma;
    float* mask = new float[maskSize];
    int*** dataIn;
    int** dataIntermediate;
    int** dataOut;
    int numRows, numColumns, maxGreyValue;

    // calculate the mask
    Gauss_1D_mask( (float) sigma, maskSize, mask );

    // extract all data from the file
    ReadImage(pathToImage.c_str(), dataIn, numRows, numColumns, maxGreyValue);

    // allocate space for dataOut
    dataOut = new int*[numColumns];

    for(int i = 0; i < numColumns; i++)
    {
      dataOut[i] = new int[numRows];
    }

    // perform horizontal smoothing across the data
    performHorizontalSmoothing( (*dataIn), dataOut, numRows, numColumns, maskSize, mask);

    // perform vertical smoothing across the data
    performHorizontalSmoothing( dataOut, dataOut, numRows, numColumns, maskSize, mask);

    // normalize the data
//  performNormalization(dataOut, numRows, numColumns, maxGreyValue);

    // adjust the output file name
    pathToImage = buildOutputFilename(pathToImage, sigma);

    // output results into a new file
    WriteImage(pathToImage.c_str(), dataOut, numRows, numColumns, maxGreyValue);
  }


  /*
   * Does the horizontal smoothing operation, taking the values from several pixels
   * and combining them into a new value for each pixel. The output pixel data is
   * stored inside the "dataOut" parameter, which is passed by reference.
   */
  void performHorizontalSmoothing(int** dataIn, int** dataOut, int& numRows, int& numColumns, int maskSize, float* mask)
  {
    int currentX, startIndexX, endIndexX;
    int currentY;
    int maskIndex;
    float sum;

    // loop through all the data in dataIn
    for(currentX = 0; currentX < numRows; currentX++)
    {

      for(currentY = 0; currentY < numColumns; currentY++)
      {
        // generate the start and end indices
        startIndexX = currentX - (maskSize / 2);
        endIndexX = currentX + (maskSize / 2);

        // set sum to 0
        sum = 0;

        // loop from the start index to the end index
        for(int indexX = startIndexX, maskIndex = 0; indexX <= endIndexX; indexX++, maskIndex++)
        {

          // check if the current index is a valid index (meaning the index is actually inside the image)
          if( (indexX >= 0 && indexX < numRows) )
          {

            // increment sum with value at dataIn[indexY][indexX] * mask[maskIndex]
            sum += ( dataIn[currentY][indexX] * mask[maskIndex] );

          }
        }

        // push sum into dataOut
        dataOut[currentY][currentX] = (int) sum;

      }

    }
  }


  /*
   * Does the vertical smoothing operation, taking the values from several pixels
   * and combining them into a new value for each pixel. The output pixel data is
   * stored inside the "dataOut" parameter, which is passed by reference.
  */
  void performVerticalSmoothing(int** dataIn, int** dataOut, int& numRows, int& numColumns, int maskSize, float* mask)
  {
    int currentX;
    int currentY, startIndexY, endIndexY;
    int maskIndex;
    float sum;

    // loop through all the data in dataIn
    for(currentX = 0; currentX < numRows; currentX++)
    {

      for(currentY = 0; currentY < numColumns; currentY++)
      {
        // generate the start and end indices
        startIndexY = currentY - (maskSize / 2);
        endIndexY = currentY + (maskSize / 2);

        // set sum to 0
        sum = 0;

        // loop from the start index to the end index
        for(int indexY = startIndexY, maskIndex = 0; indexY <= endIndexY; indexY++, maskIndex++)
        {

          // check if the current index is a valid index (meaning the index is actually inside the image)
          if( (indexY >= 0 && indexY < numRows) )
          {

            // increment sum with value at dataIn[indexY][indexY] * mask[maskIndex]
            sum += ( dataIn[indexY][currentX] * mask[maskIndex] );

          }
        }

        // push sum into dataOut
        dataOut[currentY][currentX] = (int) sum;

      }

    }
  }


  /*
   * Performs normalization across the data that will be written to file. This ensures that
   * every pixel value falls in the acceptable range.
   */
  void performNormalization(int** dataOut, int& numRows, int& numColumns, int& maxGreyValue)
  {
    int minimumValue = 0;
    int denominator = maxGreyValue - minimumValue;
    int value;


    // loop through all pixels
    for(int x = 0; x < numRows; x++)
    {
      for(int y = 0; y < numColumns; y++)
      {

        value = dataOut[y][x];

        // apply mathematical transformation to each pixel value
        dataOut[y][x] = (value - minimumValue) / denominator;

      }
    }
  }


  /*
   * Builds the output filename.
   */
  string buildOutputFilename(string& filepath, int& sigma)
  {
    string outputString = "";
    int strlength = filepath.length();
    int i = 0;

    // copy the entire string until a "." is reached
    while( filepath[i] != '.')
    {
      outputString += filepath[i];
      i++;
    }

    // insert special text into filepath
    outputString += "_sigma=" + to_string(sigma) + "_2_1D-masks";

    // finish copying filepath into the output string
    for(i; i < strlength; i++)
    {
      outputString += filepath[i];
    }

    return outputString;
  }

} // end namespace

