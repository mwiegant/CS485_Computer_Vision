#include <iostream>
#include <string>

//#include "src/Sobel_edge_detection.cpp"

using namespace std;

void displayGaussianPyramidMenu()
{
  string pathToImage;
  float initialScale;
  int numberOfLevels;

  // display menu-title (just to clarify to the user that this is the gaussian pyramid logic)
  cout << "" << endl;
  cout << "  ####################" << endl;
  cout << "  # Gaussian Pyramid #" << endl;
  cout << "  ####################" << endl;
  cout << "" << endl;

  // gather inputs
  cout << "" << endl;
  cout << "Enter the path to the image: ";
  cin >> pathToImage;
//  cout << "Enter the initial scale of the image <1.0, 0.5, 0.25, etc> : ";
  cout << "Enter the resolution scale of the first output image <1.0, 0.5, 0.25, etc> : ";
  cin >> initialScale;
  cout << "Enter how many levels you wish to generate : ";
  cin >> numberOfLevels;

  // run the Sobel edge detection, with inputs
//  performSobelEdgeDetection(pathToImage, threshold );

  cout << "Completed generation of the Gaussian Pyramid." << endl;
}