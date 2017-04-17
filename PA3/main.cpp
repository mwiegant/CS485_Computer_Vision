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
  int **fileImageData;
  int **thresholdedData;
  int **circleImageData;
  int **circleImageThresholded;
  vector<Circle> circles;

  // check for correct amount of command line arguments
  if(argc != 5) {
    cout << "ERROR - expected: <./pa3> <filepath> <threshold> <minCircleRadius> <maxCircleRadius>" << endl;
    return -1;
  }

  // read data from file
  ReadPGMImage(argv[1], &fileImageData, numRows, numColumns, maxGreyValue);

  // grab command line arguments
  threshold = atoi(argv[2]);
  minRadius = atoi(argv[3]);
  maxRadius = atoi(argv[4]);

  cout << "threshold: " << threshold << endl;
  cout << "minRadius: " << minRadius << endl;
  cout << "maxRadius: " << maxRadius << endl;
  cout << "DEBUG - STARTING -- SOBEL EDGE DETECTION." << endl << endl;

  // apply edge detection
  performSobelEdgeDetection(&fileImageData, numRows, numColumns, maxGreyValue, threshold, &thresholdedData);

  cout << "DEBUG - STARTING -- HOUGH TRANSFORM." << endl << endl;

  // apply hough transform
  circles = houghTransform(thresholdedData, numRows, numColumns, maxGreyValue, minRadius, maxRadius);

  // DEBUG - print circle data
  cout << "DEBUG - found " << circles.size() << " circles." << endl;
  string text;
  for(int i = 0; i < circles.size(); i++) {
    text = "Circle......... x: " + to_string(circles[i].x) + ", y: " + to_string(circles[i].y) + ", r: " + to_string(circles[i].r);
    cout << text << endl;
  }

  // draw detected circles over the image from file
  for(int i = 0; i < circles.size(); i++) {
    if(i == 0) {
      circleImageData = drawGreyscaleCircle(fileImageData, numRows, numColumns, circles[i].x, circles[i].y, circles[i].r, 128);
    } else {
      circleImageData = drawGreyscaleCircle(circleImageData, numRows, numColumns, circles[i].x, circles[i].y, circles[i].r, 128);
    }
  }

  // draw detected circles over the thresholded image
  for(int i = 0; i < circles.size(); i++) {
    if(i == 0) {
      circleImageThresholded = drawGreyscaleCircle(thresholdedData, numRows, numColumns, circles[i].x, circles[i].y, circles[i].r, 128);
    } else {
      circleImageThresholded = drawGreyscaleCircle(circleImageThresholded, numRows, numColumns, circles[i].x, circles[i].y, circles[i].r, 128);
    }
  }

  // write thresholded image to file
  fileOut = "images/image_threshold=" + to_string(threshold) + "_output.pgm";
  WritePGMImage(fileOut.c_str(), thresholdedData, numRows, numColumns, maxGreyValue);

  // write image with circles superimposed to file
  fileOut = "images/image_superimposed.pgm";
  WritePGMImage(fileOut.c_str(), circleImageData, numRows, numColumns, maxGreyValue);

  // write thresholded image with circles superimposed to file
  fileOut = "images/image_threshold_superimposed.pgm";
  WritePGMImage(fileOut.c_str(), circleImageThresholded, numRows, numColumns, maxGreyValue);

  cout << "INFO - Successfully completed program." << endl;

  return 0;
}

