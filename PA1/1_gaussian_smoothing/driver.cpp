#include <iostream>
#include <string>

#include "src/1D_Gaussian_Smoothing.cpp"
#include "src/2D_Gaussian_Smoothing_1_mask.cpp"
#include "src/2D_Gaussian_Smoothing_2_masks.cpp"

using namespace std;

/* function prototypes */
void process1D_Gaussian_Smoothing();
void process2D_Gaussian_Smoothing_1_mask();
void process2D_Gaussian_Smoothing_2_masks();

void displayGaussianSmoothingMenu()
{
  bool loopFlag = true;
  int numOptions = 4;
  int userChoice;

  // display menu
  cout << "" << endl;
  cout << "  ######################" << endl;
  cout << "  # Gaussian Smoothing #" << endl;
  cout << "  ######################" << endl;
  cout << "" << endl;
  cout << "Please select from one of the options below." << endl;
  cout << "" << endl;
  cout << "1 - 1D Gaussian Smoothing" << endl;
  cout << "2 - 2D Gaussian Smoothing [2D Mask]" << endl;
  cout << "3 - 2D Gaussian Smoothing [2 1D Masks]" << endl;
  cout << "4 - Exit Program." << endl;
  cout << "" << endl;
  cout << "Select an option <1,2,3,4> : ";

  // do ask user to select an option while no valid option has been presented
  do
  {
    cin >> userChoice;

    // check for valid selection
    if (userChoice > 0 && userChoice <= numOptions)
    {
      loopFlag = false;
    }
    else
    {
      cout << "Invalid Selection." << endl;
      cout << "" << endl;
      cout << "Select an option <1,2,3,4> : ";
    }

  } while (loopFlag);

  // switch on user input
  switch (userChoice)
  {
    case 1:
      process1D_Gaussian_Smoothing();
      break;

    case 2:
      process2D_Gaussian_Smoothing_1_mask();
      break;

    case 3:
      process2D_Gaussian_Smoothing_2_masks();
      break;

    case 4:
      cout << "Exiting Program." << endl;
      break;

    default:
      cout << "ERROR - Unsupported option selected in menu 1. Exiting program." << endl;
      break;
  }
}


void process1D_Gaussian_Smoothing()
{
  int sigma;
  string pathToImage;
  int returnStatus;

  // gather inputs
  cout << "" << endl;
  cout << "Enter a value for sigma: ";
  cin >> sigma;
  cout << "Enter the path to the image: ";
  cin >> pathToImage;

  // run the 1D Gaussian Smoothing, with inputs
  returnStatus = Gauss_1D_1Mask::perform1D_Gaussian_Smoothing(sigma, pathToImage);

  // check the results for any errors
  if (returnStatus == -1)
  {
    cout << "ERROR - Could not find the file located at '" << pathToImage.c_str() << "'." << endl;
  }
  else if (returnStatus != 0)
  {
    cout << "ERROR - There was an error with performing the 1D_Gaussian_Smoothing" << endl;
  }
  else
  {
    cout << "Successfully performed 1D Gaussian Smoothing. Look for an output file in the directory of your image." << endl;
  }
}


void process2D_Gaussian_Smoothing_1_mask()
{
  int sigma;
  string pathToImage;

  // gather inputs
  cout << "" << endl;
  cout << "Enter a value for sigma: ";
  cin >> sigma;
  cout << "Enter the path to the image: ";
  cin >> pathToImage;

  // run the 1D Gaussian Smoothing, with inputs
  Gauss_2D_1Mask::perform2D_Gaussian_Smoothing_1_mask(sigma, pathToImage);

  cout << "Completed 2D Gaussian Smoothing operation." << endl;
}


void process2D_Gaussian_Smoothing_2_masks()
{
  int sigma;
  string pathToImage;

  // gather inputs
  cout << "" << endl;
  cout << "Enter a value for sigma: ";
  cin >> sigma;
  cout << "Enter the path to the image: ";
  cin >> pathToImage;

  // run the 1D Gaussian Smoothing, with inputs
  Gauss_2D_2Mask::perform2D_Gaussian_Smoothing_2_masks(sigma, pathToImage);

  cout << "Completed 2D Gaussian Smoothing operation." << endl;
}

















