#include <iostream>
#include <string>

#include "src/Sobel_edge_detection.cpp"

using namespace std;

void displayEdgeDetectionMenu()
{
  string pathToImage;
  int threshold;

  // display menu-title (just to clarify to the user that this is the sobel edge detection logic)
  cout << "" << endl;
  cout << "  ########################" << endl;
  cout << "  # Sobel Edge Detection #" << endl;
  cout << "  ########################" << endl;
  cout << "" << endl;

  // gather inputs
  cout << "" << endl;
  cout << "Enter the path to the image: ";
  cin >> pathToImage;
  cout << "Enter the threshold for edge detection: ";
  cin >> threshold;

  // run the Sobel edge detection, with inputs
  Sobel_E_D::performSobelEdgeDetection(pathToImage, threshold );

  cout << "Completed the Sobel edge detection operation." << endl;
}