#include <iostream>

using namespace std;

int main()
{
  bool loopFlag = true;
  int userChoice;

  // display menu
  cout << "" << endl;
  cout << "    #############" << endl;
  cout << "    # Main Menu #" << endl;
  cout << "    #############" << endl;
  cout << "" << endl;
  cout << "Please select from one of the options below." << endl;
  cout << "" << endl;
  cout << "1 - Gaussian Smoothing" << endl;
  cout << "2 - Edge Detection" << endl;
  cout << "3 - Gaussian Pyramid" << endl;
  cout << "4 - Exit Program." << endl;
  cout << "" << endl;
  cout << "Select an option: <1,2,3,4> ";

  // do ask user to select an option while no valid option has been presented
  do
  {
    cin >> userChoice;

    // check for valid selection
    if (userChoice > 0 && userChoice <= 4)
    {
      loopFlag = false;
    }
    else
    {
      cout << "Invalid Selection." << endl;
      cout << "" << endl;
      cout << "Select an option: <1,2,3,4> ";
    }

  } while (loopFlag);

  // switch on user input
  switch (userChoice)
  {
    case 1:
      // todo - hook in gaussian smoothing
      break;

    case 2:
      // todo - hook in edge detection
      break;

    case 3:
      // todo - hook in gaussian pyramid
      break;

    case 4:
      cout << "Exiting Program." << endl;
      return 0;

    default:
      cout << "ERROR - Unsupported option selected in the main menu. Exiting program." << endl;
      return -1;
  }

  return 0;
}


/*
###########
# Main Menu
###########

Please select from one of the options below.

1 - Gaussian Smoothing
2 - Edge Detection
3 - Gaussian Pyramid
4 - Exit Program.

Selection: <1,2,3,4>
 */