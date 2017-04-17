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
  int numRows, numColumns, maxGreyValue, threshold, minRadius, maxRadius;
  int **dataIn;
  int **dataOut;
  vector<Circle> circles;

  // check for correct amount of command line arguments
  if(argc != 5) {
    cout << "ERROR - expected: <./pa3> <filepath> <threshold> <minCircleRadius> <maxCircleRadius>" << endl;
    return -1;
  }

  // read data from file
  ReadPGMImage(argv[1], &dataIn, numRows, numColumns, maxGreyValue);

  // grab command line arguments
  threshold = atoi(argv[2]);
  minRadius = atoi(argv[3]);
  maxRadius = atoi(argv[4]);

  // apply edge detection
  performSobelEdgeDetection(&dataIn, numRows, numColumns, maxGreyValue, threshold, &dataOut);

  // apply hough transform
  circles = houghTransform(dataIn, numRows, numColumns, minRadius, maxRadius);

  cout << "DEBUG - found " << circles.size() << " circles." << endl;

  // write image to file
  fileOut = "images/threshold=" + to_string(threshold) + "_output.pgm";
  WritePGMImage(fileOut.c_str(), dataOut, numRows, numColumns, maxGreyValue);

  cout << "INFO - Successfully completed program." << endl;

  return 0;
}

