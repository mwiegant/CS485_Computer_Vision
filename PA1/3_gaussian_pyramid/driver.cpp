#include <iostream>
#include <string>

#include "src/gaussian_pyramid.cpp"

using namespace std;

void displayGaussianPyramidMenu()
{
  string pathToImage;
  int sigma;
  int numberOfLevels, numberOfIntermediateLevels;

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
  cout << "Enter a value for sigma : ";
  cin >> sigma;
  cout << "Enter how many levels you wish to generate : ";
  cin >> numberOfLevels;
  cout << "Enter how many intermediate levels you wish to generate : ";
  cin >> numberOfIntermediateLevels;

  // run the Sobel edge detection, with inputs
  generateGaussianPyramid(pathToImage, sigma, numberOfLevels, numberOfIntermediateLevels );

  cout << "Completed generation of the Gaussian Pyramid." << endl;
}