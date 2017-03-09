#include <iostream>
#include <string>

#include "src/normalization.cpp"

using namespace std;

int main()
{
  string filepath;
  // 4 coordinates, each with an (x,y) means I need an array of size 8
  int *POICoordinates = new int[8];
  int normImageSizeX, normImageSizeY;
  int counter = 0;

  cout << endl;
  cout << "    ########################" << endl;
  cout << "    # Facial Normalization #" << endl;
  cout << "    ########################" << endl;
  cout << endl;

  cout << "This program assumes you will use 4 PoIs (Points of Interest)." << endl;
  cout << "Please enter the path to your image: ";
  cin >> filepath;

  // obtain PoI locations
  for(int i = 1; i < 5; i++)
  {
    cout << "PoI " << i << " X : ";
    cin >> POICoordinates[counter];
    counter++;

    cout << "PoI " << i << " Y : ";
    cin >> POICoordinates[counter];
    counter++;
  }

  // obtain normalized image size dimensions
  cout << "Please enter the desired size of your normalized image." << endl;
  cout << "NOTE: the normalized image dimensions must be less than the original image." << endl;
  cout << "X: ";
  cin >> normImageSizeX;
  cout << "Y: ";
  cin >> normImageSizeY;
  cout << endl;

  cout << "Normalizing the image..." << endl;

  performNormalization(filepath, POICoordinates, normImageSizeX, normImageSizeY);

  cout << "Successfully completed image normalization." << endl;

  return 0;
}




