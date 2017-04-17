#ifndef MAX_EDGE_DETECT
#define MAX_EDGE_DETECT

#include <string>
#include <cmath>

/* my code */
#include "file-io.cpp"
#include "data/sobel-masks.cpp"

using namespace std;

namespace maxEdgeDetect {

    const int ED_DEBUG = false;

    /* function prototypes */
    void allocate2DArrays(int** &array, int& rows, int& columns);
    void applySobelMask(int** dataIn, int** dataOut, int& numRows, int& numColumns, int** gradientMask);
    void applyGradientMagnitude(int** dataGradientX, int** dataGradientY, int** dataOut, int& numRows, int& numColumns);
    void applyThreshold(int** dataIn, int** dataOut, int& threshold, int& numRows, int& numColumns, int& maxGreyValue);
    void performNormalization(int** dataOut, int& numRows, int& numColumns, int& maxGreyValue);
    string buildOutputFilename(string& filepath, int& threshold, bool includeThreshold, string& additionalIdentifier);


    /*
     * Given the data from an image, produces an output image which contains only the
     * edges from the input which are significantly strong, according to the
     * provided threshold.
     *
     * @param dataIn - data from a GREY-SCALED image
     * @param numRows - number of rows in input data
     * @param numColumns - number of columns in input data
     * @param maxGreyValue - max grey value from input data
     * @param threshold - the threshold edges must be above to be included in the output
     * @param dataOut - the output image
     */
    void performSobelEdgeDetection(int ***dataIn, int numRows, int numColumns, int maxGreyValue, int threshold, int ***dataOut)
    {
      // variables
      int** gradientX;
      int** gradientY;
      int** dataGradientX;
      int** dataGradientY;
      int** dataGradientMagnitude;
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

      if(ED_DEBUG) cout << "DEBUG - obtained sobel mask gradients." << endl;

      // allocate space for intermediate and final data pointer arrays
      allocate2DArrays(dataGradientX, numRows, numColumns);
      allocate2DArrays(dataGradientY, numRows, numColumns);
      allocate2DArrays(dataGradientMagnitude, numRows, numColumns);
      allocate2DArrays((*dataOut), numRows, numColumns);

      if(ED_DEBUG) cout << "DEBUG - allocated space for all arrays." << endl;

      // apply mask Ix and mask Iy to the image
      applySobelMask( (*dataIn), dataGradientX, numRows, numColumns, gradientX);
      applySobelMask( (*dataIn), dataGradientY, numRows, numColumns, gradientY);

      if(ED_DEBUG) cout << "DEBUG - applied sobel mask to generate gradients." << endl;

      // apply the gradient magnitude M(x,y) to the image
      applyGradientMagnitude(dataGradientX, dataGradientY, dataGradientMagnitude, numRows, numColumns);

      if(ED_DEBUG) cout << "DEBUG - generated the gradient magnitude." << endl;

      // apply normalization
      performNormalization(dataGradientMagnitude, numRows, numColumns, maxGreyValue);

      if(ED_DEBUG) cout << "DEBUG - normalized all cells in the gradient magnitude." << endl;

      // apply thresholding to get the final image
      applyThreshold(dataGradientMagnitude, (*dataOut), threshold, numRows, numColumns, maxGreyValue);

      if(ED_DEBUG) cout << "DEBUG - applied threshold; generated output of edge detection func." << endl;

      // optimization, set edges of dataset to 0, so they are not detected as edges
      for(int y = 0; y < numColumns; y++) {
        for(int x = 0; x < numRows; x++) {
          if( (x == 0) || (y == 0) || (x+1 == numRows) || (y+1 == numColumns) )
            (*dataOut)[y][x] = 0;
        }
      }

      if(ED_DEBUG) cout << "DEBUG - removed detected edges from edges of dataset." << endl;

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
      for(currentY = 0; currentY < numColumns; currentY++) {
        for(currentX = 0; currentX < numRows; currentX++) {

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
      for(int y = 0; y < numColumns; y++) {
        for(int x = 0; x < numRows; x++) {

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
      for(int y = 0; y < numColumns; y++) {
        for(int x = 0; x < numRows; x++) {

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
      float minimumValue = 0;
      int maximumValue = 0;
      int range;
      int value;
      float difference;
      int scalingFactor = maxGreyValue;

      // loop through all pixels, find largest value
      for(int y = 0; y < numColumns; y++) {
        for (int x = 0; x < numRows; x++) {
          if(dataOut[y][x] > maximumValue)
            maximumValue = dataOut[y][x];
        }
      }

      // calculate range
      range = maximumValue - minimumValue;

      // loop through all pixels, apply normalization
      for(int y = 0; y < numColumns; y++) {
        for(int x = 0; x < numRows; x++) {

          value = dataOut[y][x];
          difference = value - minimumValue;

          // apply mathematical transformation to each pixel value
          dataOut[y][x] = int( scalingFactor * difference / range );

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
#endif