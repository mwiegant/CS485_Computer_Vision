#include "src/1D_Gaussian_Smoothing.cpp"

#include <iostream>

using namespace std;

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
      // todo - hook in 1D gaussian smoothing
      break;

    case 2:
      // todo - 2D Gaussian Smoothing [2D Mask]
      break;

    case 3:
      // todo - 2D Gaussian Smoothing [2 1D Masks]
      break;

    case 4:
      cout << "Exiting Program." << endl;
      break;

    default:
      cout << "ERROR - Unsupported option selected in menu 1. Exiting program." << endl;
      break;
  }
}