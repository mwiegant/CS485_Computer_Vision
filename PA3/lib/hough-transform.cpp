#ifndef MAX_HOUGH_TRANSFORM
#define MAX_HOUGH_TRANSFORM
#define PI 3.14159

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

namespace maxHoughTransform {

  /* structures */
  struct Circle {
      int x;
      int y;
      int r;
  };

  /* function prototypes */
//  void recordAccumulatorVotes(int ****accumulator, int ***dataIn);


  vector<Circle> houghTransform(int **dataIn, int dataX, int dataY, int minRadius, int maxRadius)
  {
    // var initialization
    int *** accumulator;
    int radiusRange = maxRadius - minRadius + 1;
    int a, b, rI;
    int maxVotes;
    float percentOfMaximum = 0.8;
    int cutoff;
    float rads;
    Circle circle;
    vector<Circle> circles;

    cout << "DEBUG - start of hough transform" << endl;

    // fill accumulator array with 0s
    // dimensions: [-dataX..dataX][-dataY..dataY][minRadius..maxRadius]
    accumulator = new int**[dataX * 2];
    for(int i = dataX * -2; i < dataX * 2; i++) {

      accumulator[i] = new int*[dataY * 2];
      for(int j = dataY * -2; j < dataY * 2; j++) {

        accumulator[i][j] = new int[radiusRange];
        for(int k = minRadius; k <= radiusRange; k++) {

          accumulator[i][j][k] = 0;
        }
      }
    }

    cout << "DEBUG - finished filling accumulator array with 0s" << endl;

    // do voting in parameter space
    for(int x = 0; x < dataX; x++) {
      for(int y = 0; y < dataY; y++) {

        // only do voting if the (x,y) location correspond to an edgepoint
        if(dataIn[x][y] > 0) {

          for(int r = minRadius; r <= maxRadius; r++) {
            for(int theta = 0; theta <= 360; theta++) {
              rads = theta * PI / 180;

              // where (a,b) represent the center of a circle
              a = int( x - (r * cos(rads)));
              b = int( y - (r * sin(rads)));
              rI = r - minRadius;

              // error checking for out of range votes
              if( (abs(float(a)) >= dataX * 2) || (abs(float(b)) >= dataY * 2) ) {
                cout << "ERROR - tabulating a vote that will be out of range of accumulator array" << endl
                     << "a: " << a << ", b: " << b << ", r: " << r  << ", rI: " << rI << endl;
              } else {
                accumulator[a][b][rI]++;
              }

            }
          }

        }
      }
    }

    cout << "DEBUG - finished voting" << endl;

    // find maximum value in parameter space
    maxVotes = 1;

    for(int x = dataX * -1; x < dataX; x++) {
      for(int y = dataY * -1; y < dataY; y++) {
        for(int r = 0; r < radiusRange; r++) {
          if (accumulator[x][y][r] > maxVotes)
            maxVotes = accumulator[x][y][r];
        }
      }
    }

    cout << "DEBUG: maxVotes: " << maxVotes << endl;
    cutoff = int(maxVotes * percentOfMaximum);

    // thin out regions of cells with passing votes
    // todo - implement thinning (for now just doing relative thresholding)
    for(int x = dataX * -1; x < dataX; x++) {
      for(int y = dataY * -1; y < dataY; y++) {
        for(int r = 0; r < radiusRange; r++) {

          // relative thresholding
          if (accumulator[x][y][r] > cutoff) {
            circle.x = x; circle.y = y; circle.r = r + minRadius;
            circles.push_back(circle);
          }
        }
      }
    }

    // return [x,y,r] of cells that represent circles (after voting and thinning)
    return circles;
  }








}
#endif