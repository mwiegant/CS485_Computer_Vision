#include <string>
#include <cmath>

#include "ReadImage.cpp"
#include "WriteImage.cpp"
#include "Sobel_masks.cpp"

namespace Sobel_E_D {

    /* function prototypes */
    void allocate2DArrays(int** &array, int& rows, int& columns);
    void applySobelMask(int** dataIn, int** dataOut, int& numRows, int& numColumns, int** gradientMask);
    void applyGradientMagnitude(int** dataGradientX, int** dataGradientY, int** dataOut, int& numRows, int& numColumns);
    void applyThreshold(int** dataIn, int** dataOut, int& threshold, int& numRows, int& numColumns, int& maxGreyValue);
    void performNormalization(int** dataOut, int& numRows, int& numColumns, int& maxGreyValue);
    string buildOutputFilename(string& filepath, int& threshold, bool includeThreshold, string& additionalIdentifier);


    void performSobelEdgeDetection(string pathToImage, int threshold)
    {
      // variables
      int*** dataIn;
      int** gradientX;
      int** gradientY;
      int** dataGradientX;
      int** dataGradientY;
      int** dataGradientMagnitude;
      int** dataOut;
      int numRows, numColumns, maxGreyValue;
      string pathGradientX, pathGradientY, pathGradientMagnitude, pathThreshold;
      string gradientXIdentifier = "gradient_X";
      string gradientYIdentifier = "gradient_Y";
      string gradientMagnitudeIdentifier = "gradient_magnitude";
      string thresholdIdentifier = "edges";
      bool includeThreshold = true;
      bool excludeThreshold = false;

      // get the X and Y gradients
      getSobelMask_X(gradientX);
      getSobelMask_Y(gradientY);

      // get the initial image from file
      ReadImage(pathToImage.c_str(), dataIn, numRows, numColumns, maxGreyValue);

      // allocate space for intermediate and final data pointer arrays
      allocate2DArrays(dataGradientX, numRows, numColumns);
      allocate2DArrays(dataGradientY, numRows, numColumns);
      allocate2DArrays(dataGradientMagnitude, numRows, numColumns);
      allocate2DArrays(dataOut, numRows, numColumns);

      // apply mask Ix and mask Iy to the image
      applySobelMask( (*dataIn), dataGradientX, numRows, numColumns, gradientX);
      applySobelMask( (*dataIn), dataGradientY, numRows, numColumns, gradientY);

      // apply the gradient magnitude M(x,y) to the image
      applyGradientMagnitude(dataGradientX, dataGradientY, dataGradientMagnitude, numColumns, numRows);

      // apply thresholding to get the final image
      applyThreshold(dataGradientMagnitude, dataOut, threshold, numRows, numColumns, maxGreyValue);

      // perform normalizations
//  performNormalization(dataGradientX, numRows, numColumns, maxGreyValue);
//  performNormalization(dataGradientY, numRows, numColumns, maxGreyValue);
//  performNormalization(dataGradientMagnitude, numRows, numColumns, maxGreyValue);
//  performNormalization(dataOut, numRows, numColumns, maxGreyValue);

      // generate the output file names
      pathGradientX = buildOutputFilename(pathToImage, threshold, excludeThreshold, gradientXIdentifier);
      pathGradientY = buildOutputFilename(pathToImage, threshold, excludeThreshold, gradientYIdentifier);
      pathGradientMagnitude = buildOutputFilename(pathToImage, threshold, excludeThreshold, gradientMagnitudeIdentifier);
      pathThreshold = buildOutputFilename(pathToImage, threshold, includeThreshold, thresholdIdentifier);

      // write the final images to file
      WriteImage(pathGradientX.c_str(), dataGradientX, numRows, numColumns, maxGreyValue);
      WriteImage(pathGradientY.c_str(), dataGradientY, numRows, numColumns, maxGreyValue);
      WriteImage(pathGradientMagnitude.c_str(), dataGradientMagnitude, numRows, numColumns, maxGreyValue);
      WriteImage(pathThreshold.c_str(), dataOut, numRows, numColumns, maxGreyValue);
    }


/*
   * Allocates space for 2D arrays, given an "array".
   *
   * Allocates space given the dimensions "rows" and "columns",
   * where the first dimension will be the number of columns
   * and the second dimension will be the number of rows in each column.
   */
    void allocate2DArrays(int** &array, int& rows, int& columns)
    {
      array = new int*[columns];

      for(int i = 0; i < columns; i++)
        array[i] = new int[rows];
    }


/*
 * Applies the "gradientMask" Sobel Mask to "dataIn" in order to generate
 * "dataOut".
 *
 * Keep in mind that Sobel Masks are typically 3x3 masks.
 */
    void applySobelMask(int** dataIn, int** dataOut, int& numRows, int& numColumns, int** gradientMask)
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
          // the +1 / -1 is done because Sobel Masks are 3x3 masks
          startIndexX = currentX - 1;
          endIndexX = currentX + 1;
          startIndexY = currentY - 1;
          endIndexY = currentY + 1;

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

                // increment sum with value at dataIn[indexY][indexX] * gradientMask[maskIndexY][maskIndexX]
                sum += ( dataIn[indexY][indexX] * gradientMask[maskIndexY][maskIndexX] );

              }

            }
          }

          // push sum into dataOut
          dataOut[currentY][currentX] = (int) sum;

        }

      }
    }


/*
 * Computes the gradient magnitude and stores in "dataOut".
 */
    void applyGradientMagnitude(int** dataGradientX, int** dataGradientY, int** dataOut, int& numRows, int& numColumns)
    {
      // variables
      int magnitude;

      // loop through every pixel
      for(int x = 0; x < numRows; x++)
      {
        for(int y = 0; y < numColumns; y++)
        {
          // compute: magnitude = sqrt( (dataGradientX[y][x] ^ 2) + (dataGradientY[y][x] ^ 2) )
          magnitude = sqrt( pow(dataGradientX[y][x], 2) + pow(dataGradientY[y][x], 2) );

          // store magnitude in dataOut[y][x]
          dataOut[y][x] = magnitude;

        }
      }

    }


/*
 * Looks at each pixel in the input, "dataIn". If the pixel's
 * value is greater than or equal to "threshold", the pixel's value
 * is adjusted to "maxGreyValue".
 *
 * If the pixel's value is less than "threshold", the pixel's value
 * is adjusted to 0.
 */
    void applyThreshold(int** dataIn, int** dataOut, int& threshold, int& numRows, int& numColumns, int& maxGreyValue)
    {

      // loop through all pixels
      for(int x = 0; x < numRows; x++)
      {
        for(int y = 0; y < numColumns; y++)
        {

          // check if dataIn[y][x] is >= threshold
          if( dataIn[y][x] >= threshold)
          {
            // set dataOut[y][x] to maxGreyValue
            dataOut[y][x] = maxGreyValue;
          }
          else
          {
            // set dataOut[y][x] to 0
            dataOut[y][x] = 0;
          }

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

          // check if data[y][x] is less than 0, and if it is set it to 0
          if( dataOut[y][x] < 0 )
          {
            dataOut[y][x] = 0;
          }
        }
      }
    }


/*
 * Builds the output filename. Appends "additionalIdentifier" to the end of
 * the filepath so I can add unique labels to each of the images that will be
 * printed out by this program.
 *
 * If "includeThreshold" is set to false, the threshold value will be omitted
 * from the output string.
 */
    string buildOutputFilename(string& filepath, int& threshold, bool includeThreshold, string& additionalIdentifier)
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

      // insert special text into filepath, either including or excluding the threshold
      if(includeThreshold)
        outputString += "_threshold=" + to_string(threshold) + "_Sobel_" + additionalIdentifier;
      else
        outputString += "_Sobel_" + additionalIdentifier;

      // finish copying filepath into the output string
      for(i; i < strlength; i++)
      {
        outputString += filepath[i];
      }

      return outputString;
    }

}

