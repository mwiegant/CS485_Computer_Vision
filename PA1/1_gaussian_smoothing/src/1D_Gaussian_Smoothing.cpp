#include <fstream>
#include <vector>

#include "Gauss_masks.c"

using namespace std;

/* function prototypes */
bool readInData(const char* pathToData, vector<float>& dataIn);
void performSmoothing(vector<float>& dataIn, vector<float>& dataOut, int maskSize, float* mask);
void outputDataToFile(const char* outputDataPath, vector<float>& dataOut);

/*
 * Performs 1D Gaussian Smoothing by calling helper functions to perform
 * the various steps required.
 *
 * If any step fails, an error is returned.
 */
int perform1D_Gaussian_Smoothing(int sigma, string pathToImage)
{
  int returnStatus = 0;
  int maskSize = 5 * sigma;
  float* mask = new float[maskSize];
  vector<float> dataIn;
  vector<float> dataOut;
  bool readDataFlag;


  // calculate the mask
  Gauss_1D_mask( (float) sigma, maskSize, mask );

  // extract all data from the file
  readDataFlag = readInData( pathToImage.c_str(), dataIn);

  // error out if no data was read from the file
  if (!readDataFlag)
  {
    return -1;
  }

  // perform smoothing across the data
  performSmoothing(dataIn, dataOut, maskSize, mask);

  // adjust the output file name
  pathToImage += "_sigma=" + to_string(sigma);

  // output results into a new file
  outputDataToFile(pathToImage.c_str(), dataOut);

  // return success
  return returnStatus;
}


/*
 * Reads in the data from file. Returns success if any data was
 * read from the file.
 */
bool readInData(const char* pathToData, vector<float>& dataIn)
{
  ifstream fin;
  float data;
  bool readTheFile = false;

  // clear input file-stream flags and open the file
  fin.clear();
  fin.open(pathToData);

  while( fin.good() )
  {
    readTheFile = true;

    fin >> data;

    dataIn.push_back(data);
  }

  fin.close();

  return readTheFile;
}


/*
 * Does the actual smoothing operation, taking the values from several pixels
 * and combining them into a new value for each pixel. The output pixel data is
 * stored inside the "dataOut" parameter, which is passed by reference.
 */
void performSmoothing(vector<float>& dataIn, vector<float>& dataOut, int maskSize, float* mask)
{
  int dataSize = dataIn.size();
  int currentPixel, startIndex, endIndex, maskIndex;
  float sum;

  // loop through all the data in dataIn
  for(currentPixel = 0; currentPixel < dataSize; currentPixel++)
  {

    // generate the start index and end index
    startIndex = currentPixel - (maskSize / 2);
    endIndex = currentPixel + (maskSize / 2);

    // set sum and maskIndex to 0
    maskIndex = 0;
    sum = 0;

    // loop from the start index to the end index
    for(int index = startIndex; index <= endIndex; index++, maskIndex++)
    {
      // check if the current index is a valid index ( > 0 && < dataSize )
      if( index > 0 && index < dataSize)
      {
        // increment sum with value at dataIn[index] * mask[maskIndex]
        sum += ( dataIn[index] * mask[maskIndex] );
      }
    }

    // push sum into dataOut
    dataOut.push_back(sum);

  }
}


/*
 * Writes the output data to file.
 */
void outputDataToFile(const char* outputDataPath, vector<float>& dataOut)
{
  ofstream fout;

  fout.clear();
  fout.open( outputDataPath );

  // write data to file
  for(int i = 0; i < dataOut.size(); i++)
  {
    fout << dataOut[i] << endl;
  }

  fout.close();
}
