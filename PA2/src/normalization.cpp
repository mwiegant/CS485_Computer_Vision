#include <iostream>
#include <string>
#include <cmath>

#include "ReadImage.cpp"
#include "WriteImage.cpp"
#include "solve_system.c"

using namespace std;

/* function prototypes */
void allocate2DArrays(int** &array, int rows, int columns);
void allocate2DArrays(float** &array, int rows, int columns);
void copyImage( int **dataIn, int& numRows, int& numColumns, int **dataOut);
void createPMatrix(float **P, int POICoordinates[]);
void createPxPyMatrices(float *Px, float *Py);
void computeTheAMatrix(float **A, float *c1, float *c2);
void applyAffineTransformation( int **dataIn, int &numRows, int &numColumns, float **A, int **dataOut );
void clipImage( int **dataIntermediate, int &normImageSizeX, int &normImageSizeY, int **dataOut );
string buildOutputFilename(string& filepath);


/*
 * Perform normalization. This program assumes 4 Points of Interest.
 */
void performNormalization(string pathToImage, int POICoordinates[], int& normImageSizeX, int& normImageSizeY)
{
  // variables
  int ***dataIn;
  int **dataIntermediate;
  int **dataOut;
  float **P, *Px, *Py, *c1, *c2, **A;
  int numRows, numColumns, maxGreyValue;
  string outputPath;

  // read in the image
  ReadImage(pathToImage.c_str(), dataIn, numRows, numColumns, maxGreyValue);

  // allocate space for matrices
  allocate2DArrays(dataIntermediate, numRows, numColumns);
  allocate2DArrays(dataOut, normImageSizeX, normImageSizeY);
  allocate2DArrays(P, 4, 3);
  allocate2DArrays(A, 3, 3);

  Px = new float[4];
  Py = new float[4];
  c1 = new float[3];
  c2 = new float[3];

  // copy the image from dataIn into dataOut
  copyImage( (*dataIn), numRows, numColumns, dataIntermediate);

  // create the P matrix
  createPMatrix(P, POICoordinates);

  // create the Px and Py matrices
  createPxPyMatrices(Px, Py);

  // apply SVD to find c1   recall: P * c1 = Px
  solve_system(4, 3, P, c1, Px);

  // apply SVD to find c2   recall: P * c2 = Py
  solve_system(4, 3, P, c2, Py);

  // compute A using c1 and c2
  computeTheAMatrix(A, c1, c2);

  // apply the affine transformation defined by A to every (x,y) in the image
  applyAffineTransformation( (*dataIn), numRows, numColumns, A, dataIntermediate );

  // clip the image in dataOut to the desired dimensions
  clipImage( dataIntermediate, normImageSizeX, normImageSizeY, dataOut );

  // generate the output file name
  outputPath = buildOutputFilename(pathToImage);

  // write the image to file
  WriteImage(outputPath.c_str(), dataOut, numRows, numColumns, maxGreyValue);

}


/*
 * Allocates space for 2D arrays of ints, given an "array".
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
 * Allocates space for 2D arrays of floats, given an "array".
 *
 * Allocates space given the dimensions "rows" and "columns",
 * where the first dimension will be the number of columns
 * and the second dimension will be the number of rows in each column.
 */
void allocate2DArrays(float** &array, int rows, int columns)
{
  array = new float*[columns];

  for(int i = 0; i < columns; i++)
    array[i] = new float[rows];
}


/*
 * Copies the image from dataIn onto dataOut.
 */
void copyImage( int **dataIn, int& numRows, int& numColumns, int **dataOut)
{
  // loop through every (x,y) in the image from dataIn
  for( int i = 0; i < numRows; i++ )
  {
    for( int j = 0; j < numColumns; j++ )
    {
      dataOut[j][i] = dataIn[j][i];
    }
  }
}


/*
 *     [ x1  y1  1 ]
 *     [ x2  y2  1 ]
 * P = [ x3  y3  1 ]
 *     [ x4  y4  1 ]
 */
void createPMatrix(float **P, int POICoordinates[])
{
  // P[y][x]

  // x1  y1  1
  P[0][0] = 1.0 * POICoordinates[0];
  P[0][1] = 1.0 * POICoordinates[1];
  P[0][2] = 1.0 * 1;

  // x2  y2 1
  P[1][0] = 1.0 * POICoordinates[2];
  P[1][1] = 1.0 * POICoordinates[3];
  P[1][2] = 1.0 * 1;

  // x3  y3  1
  P[2][0] = 1.0 * POICoordinates[4];
  P[2][1] = 1.0 * POICoordinates[5];
  P[2][2] = 1.0 * 1;

  // x4  y4  1
  P[3][0] = 1.0 * POICoordinates[6];
  P[3][1] = 1.0 * POICoordinates[7];
  P[3][2] = 1.0 * 1;
}


/*
 *       [ x1F ]       [ y1F ]
 *       [ x2F ]       [ y2F ]
 *  Px = [ x3F ]  Py = [ y3F ]
 *       [ x4F ]       [ y4F ]
 *
 * Assume window size of 48 x 40
 *
 * P1F = ( 15, 15 )
 * P2F = ( 33, 15 )
 * P3F = ( 24, 20 )
 * P4F = ( 24, 30 )
 */
void createPxPyMatrices(float *Px, float *Py)
{
  // P[x]

  Px[0] = 15.0;
  Px[1] = 33.0;
  Px[2] = 24.0;
  Px[3] = 24.0;

  Py[0] = 15.0;
  Py[1] = 15.0;
  Py[2] = 20.0;
  Py[3] = 30.0;
}


/*
 *      [ c1[0] c1[1] c1[2] ]
 *  A = [ c2[0] c2[1] c2[2] ]
 *      [  0     0     1    ]
 *
 */
void computeTheAMatrix(float **A, float *c1, float *c2)
{
  // A[y][x]

  // [ c1[0] c1[1] c1[2] ]
  A[0][0] = c1[0];
  A[0][1] = c1[1];
  A[0][2] = c1[2];

  // [ c2[0] c2[1] c2[2] ]
  A[1][0] = c2[0];
  A[1][1] = c2[1];
  A[1][2] = c2[2];

  // [  0     0     1    ]
  A[2][0] = 0;
  A[2][1] = 0;
  A[2][2] = 1;
}


/*
 * Applies the affine transformation to each pixel from
 * the input image to form the output image.
 */
void applyAffineTransformation( int **dataIn, int &numRows, int &numColumns, float **A, int **dataOut )
{
  // variables
  int newX, newY;

  // loop through every (x,y) in the image from dataIn
  for( int x = 0; x < numRows; x++ )
  {
    for( int y = 0; y < numColumns; y++ )
    {

      // compute the affine transformation location for each (x,y)
      newX = (int) roundf( (A[0][0] * x) + (A[0][1] * y) + A[0][2] );
      newY = (int) roundf( (A[1][0] * x) + (A[1][1] * y) + A[1][2] );

      // check if newX and newY are within the boundaries of the image
      if( (newX >= 0 && newX < numRows) && (newY >= 0 && newY < numColumns) )
      {
        // take the image value from dataIn[y][x] and insert it at dataOut[newY][newX]
        dataOut[newY][newX] = dataIn[y][x];
      }

    }
  }
}


/*
 * Clips the larger dataIntermediate image data into a smaller
 * image with the dimensions of normImageSizeX by normImageSizeY.
 *
 * I'm making the (potentially VERY unsafe) assumption that dataIntermediate
 * will be larger than the normalized image.
 */
void clipImage( int **dataIntermediate, int &normImageSizeX, int &normImageSizeY, int **dataOut )
{
  // copy a select portion of the data from dataIntermediate into dataOut
  for( int x = 0; x < normImageSizeX; x++ )
  {
    for( int y = 0; y < normImageSizeY; y++ )
    {
      dataOut[y][x] = dataIntermediate[y][x];
    }
  }
}


/*
 * Builds the output filename by appending "_normalized" to
 * the end of the filepath.
 */
string buildOutputFilename(string& filepath)
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

  // append "_normalized" to filepath
  outputString += "_normalized";

  // finish copying filepath into the output string
  for(i; i < strlength; i++)
  {
    outputString += filepath[i];
  }

  return outputString;
}














