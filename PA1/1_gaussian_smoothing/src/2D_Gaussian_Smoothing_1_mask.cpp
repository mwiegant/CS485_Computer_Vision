#include <fstream>

#include "Gauss_masks.c"
#include "ReadImage.cpp"
#include "WriteImage.cpp"

using namespace std;

namespace Gauss_2D_1Mask {

    /* function prototypes */
    void performSmoothing(int*** dataIn, int** dataOut, int& numRows, int& numColumns, int maskSize, float** mask);
    void performNormalization(int** dataOut, int& numRows, int& numColumns, int& maxGreyValue);
    string buildOutputFilename(string& filepath, int& sigma);


/*
 * Performs 2D Gaussian Smoothing by calling helper functions to perform
 * the various steps required.
 *
 * If any step fails, an error is returned.
 */
    void perform2D_Gaussian_Smoothing_1_mask(int sigma, string pathToImage)
    {
      int returnStatus = 0;
      int maskSize = 5 * sigma;
      float** mask;
      int*** dataIn;
      int** dataOut;
      int numRows, numColumns, maxGreyValue;

      // allocate space for the mask
      mask = new float*[maskSize];

      for(int i = 0; i < maskSize; i++)
      {
        mask[i] = new float[maskSize];
      }

      // calculate the mask
      Gauss_2D_mask( (float) sigma, maskSize, maskSize, mask );

      // extract all data from the file
      ReadImage(pathToImage.c_str(), dataIn, numRows, numColumns, maxGreyValue);

      // allocate space for dataOut
      dataOut = new int*[numColumns];

      for(int i = 0; i < numColumns; i++)
      {
        dataOut[i] = new int[numRows];
      }

      // perform smoothing across the data
      performSmoothing(dataIn, dataOut, numRows, numColumns, maskSize, mask);

      // normalize the data
//  performNormalization(dataOut, numRows, numColumns, maxGreyValue);

      // adjust the output file name
      pathToImage = buildOutputFilename(pathToImage, sigma);

      // output results into a new file
      WriteImage(pathToImage.c_str(), dataOut, numRows, numColumns, maxGreyValue);
    }


/*
 * Does the actual smoothing operation, taking the values from several pixels
 * and combining them into a new value for each pixel. The output pixel data is
 * stored inside the "dataOut" parameter, which is passed by reference.
 */
    void performSmoothing(int*** dataIn, int** dataOut, int& numRows, int& numColumns, int maskSize, float** mask)
    {
      int currentX, startIndexX, endIndexX, maskIndexX;
      int currentY, startIndexY, endIndexY, maskIndexY;
      float sum;

      // loop through all the data in dataIn
      for(currentX = 0; currentX < numRows; currentX++)
      {

        for(currentY = 0; currentY < numColumns; currentY++)
        {
          // generate the start and end indices
          startIndexX = currentX - (maskSize / 2);
          endIndexX = currentX + (maskSize / 2);
          startIndexY = currentY - (maskSize / 2);
          endIndexY = currentY + (maskSize / 2);

          // set sum to 0
          sum = 0;

          // loop from the start index to the end index
          for(int indexX = startIndexX, maskIndexX = 0; indexX <= endIndexX; indexX++, maskIndexX++)
          {
            for(int indexY = startIndexY, maskIndexY = 0; indexY <= endIndexY; indexY++, maskIndexY++)
            {

              // check if the current index is a valid index (meaning the index is actually inside the image)
              if( (indexX >= 0 && indexX < numRows) && (indexY >= 0 && indexY < numColumns) )
              {

                // increment sum with value at dataIn[indexY][indexX] * mask[maskIndexY][maskIndexX]
                sum += ( (*dataIn)[indexY][indexX] * mask[maskIndexY][maskIndexX] );

              }

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
      outputString += "_sigma=" + to_string(sigma) + "_1_2D-mask";

      // finish copying filepath into the output string
      for(i; i < strlength; i++)
      {
        outputString += filepath[i];
      }

      return outputString;
    }

}

