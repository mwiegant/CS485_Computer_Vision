#include "iostream"

/* my code */
#include "lib/file-io.cpp"
#include "lib/edge-detect.cpp"
#include "lib/hough-transform.cpp"

using namespace std;
using namespace maxEdgeDetect;
using namespace maxHoughTransform;

int main(int argc, char** argv)
{
  // var init
  string fileOut;
  int numRows, numColumns, maxGreyValue, threshold;
  int **dataIn;
  int **dataOut;

  if(argc != 3) {
    cout << "ERROR - expected: <./pa3> <filepath> <threshold>" << endl;
    return -1;
  }

  // read data from file
  ReadPGMImage(argv[1], &dataIn, numRows, numColumns, maxGreyValue);
  threshold = atoi(argv[2]);

  // apply edge detection
  performSobelEdgeDetection(&dataIn, numRows, numColumns, maxGreyValue, threshold, &dataOut);

  // apply hough transform
    //todo

  // write image to file
  fileOut = "images/threshold=" + to_string(threshold) + "_output.pgm";
  WritePGMImage(fileOut.c_str(), dataOut, numRows, numColumns, maxGreyValue);

  cout << "INFO - Successfully completed program." << endl;

  return 0;
}

