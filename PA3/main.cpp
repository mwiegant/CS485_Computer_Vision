#include "iostream"

/* my code */
#include "lib/edge-detect.cpp"
#include "lib/pgm-file-io.cpp"

using namespace std;
using namespace maxSobel;

int main(int argc, char** argv)
{
  // var init
  string fileOut = "images/output.pgm";
  int numRows, numColumns, maxGreyValue;
  int **dataIn;
  int **dataOut;
  int threshold = 40;

  if(argc != 2) {
    cout << "ERROR - must provide image filepath as second argument." << endl;
    return -1;
  }

  cout << "DEBUG - about to read in image from file." << endl;

  // read data from file
  ReadImage(argv[1], &dataIn, numRows, numColumns, maxGreyValue);

  cout << "DEBUG - read image from file." << endl;

  // apply edge detection
  performSobelEdgeDetection(&dataIn, numRows, numColumns, maxGreyValue, threshold, &dataOut);

  cout << "DEBUG - about to write data to file." << endl;

  // write image to file, to prove this even works
  WriteImage(fileOut.c_str(), dataOut, numRows, numColumns, maxGreyValue);

  cout << "INFO - Successfully completed program." << endl;

  return 0;
}

