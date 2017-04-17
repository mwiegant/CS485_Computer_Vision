#include "iostream"

/* my code */
#include "lib/file-io.cpp"
#include "lib/edge-detect.cpp"
#include "lib/hough-transform.cpp"
#include "lib/draw.cpp"

using namespace std;
using namespace maxEdgeDetect;
using namespace maxHoughTransform;
using namespace maxDraw;

int main(int argc, char** argv)
{
  // var init
  string fileOut;
  int numRows, numColumns, maxGreyValue, threshold, minRadius, maxRadius;
  int **dataIn;
  int **dataOut;
  int **dataWithCircles;
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

  // DEBUG - print circle data
  cout << "DEBUG - found " << circles.size() << " circles." << endl;
  string text;
  for(int i = 0; i < circles.size(); i++) {
    text = "Circle......... x: " + to_string(circles[i].x) + ", y: " + to_string(circles[i].y) + ", r: " + to_string(circles[i].r);
    cout << text << endl;
  }

  // draw detected circles onto the output image
  // todo - draw all cicles, not just first 10
  for(int i = 0; i < 10; i++) {
    if(i == 0) {
      dataWithCircles = drawGreyscaleCircle(dataOut, numRows, numColumns, circles[i].x, circles[i].y, circles[i].r, 128);
    } else {
      dataWithCircles = drawGreyscaleCircle(dataWithCircles, numRows, numColumns, circles[i].x, circles[i].y, circles[i].r, 128);
    }
  }

  // write thresholded image to file
  fileOut = "images/threshold=" + to_string(threshold) + "_output.pgm";
  WritePGMImage(fileOut.c_str(), dataOut, numRows, numColumns, maxGreyValue);

  // write image with circles to file
  fileOut = "images/image_circles.pgm";
  WritePGMImage(fileOut.c_str(), dataWithCircles, numRows, numColumns, maxGreyValue);

  cout << "INFO - Successfully completed program." << endl;

  return 0;
}

