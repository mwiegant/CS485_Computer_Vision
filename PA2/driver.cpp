#include <iostream>
#include <string>

using namespace std;

const int NUM_POIS = 4; // number of Points of Interests

/* function prototypes */
void processImage(string filepath, int numPOIs, int POICoordinates[]);


int main()
{
  string filepath;
  int *POICoordinates = new int[2 * NUM_POIS];
  int counter = 0;

  cout << endl;
  cout << "    ########################" << endl;
  cout << "    # Facial Normalization #" << endl;
  cout << "    ########################" << endl;
  cout << endl;

  cout << "Please enter the path to your image: ";
  cin >> filepath;

  cout << "Please enter the PoI (Point of Interest) coordinates." << endl;

  for(int i = 1; i <= NUM_POIS; i++)
  {
    cout << "PoI " << i << " X : ";
    cin >> POICoordinates[counter];
    counter++;

    cout << "PoI " << i << " Y : ";
    cin >> POICoordinates[counter];
    counter++;
  }

  cout << "Normalizing the image..." << endl;

  processImage(filepath, NUM_POIS, POICoordinates);

  cout << "Successfully completed image normalization." << endl;

  return 0;
}


void processImage(string filepath, int numPOIs, int POICoordinates[])
{

}


