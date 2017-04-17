#ifndef MAX_HOUGH_TRANSFORM
#define MAX_HOUGH_TRANSFORM
#define PI 3.14159

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

namespace maxHoughTransform {

  const int HT_DEBUG = false;

  /* structures */
  struct Circle {
      int x;
      int y;
      int r;
  };

  /* function prototypes */
  vector<Circle> doVoting(int ***data3D, int minRadius, int radiusRange, int xRange, int yRange, int cutoff);
  void doHorizontalThinning(int ***data2D, int xRange, int yRange, int cutoff);
  void doVerticalThinning(int ****data3D, int radiusRange, int xRange, int yRange, int cutoff);


    vector<Circle> houghTransform(int **dataIn, int dataNumRows, int dataNumCols, int maxGreyValue, int minRadius, int maxRadius)
  {
    // var initialization
    int *** accumulator;
    int radiusRange = maxRadius - minRadius + 1;
    int yRange = dataNumCols * 2;
    int xRange = dataNumRows * 2;
    int yMid = yRange / 2;
    int xMid = xRange / 2;
    int maxVotes = 1;
    int a, b;
    int rI, yI, xI;
    float percentOfMaximum = 0.5;
    int cutoff;
    float rads;
    vector<Circle> circles;

    if(HT_DEBUG) cout << "DEBUG - start of hough transform" << endl;

    // fill accumulator array with 0s
    // dimensions: [0..radiusRange][0..yRange][0..xRange]
    accumulator = new int**[radiusRange];

    for(int r = 0; r < radiusRange; r++) {

      accumulator[r] = new int*[yRange];
      for(int y = 0; y < yRange; y++) {

        accumulator[r][y] = new int[xRange];
        for(int x = 0; x < xRange; x++) {

          // set each element to 0
          accumulator[r][y][x] = 0;
        }
      }
    }

    if(HT_DEBUG) cout << "DEBUG - finished filling accumulator array with 0s" << endl;
    if(HT_DEBUG) cout << "DEBUG - radiusRange: 0 to " << radiusRange << ", yRange: 0 to " << yRange
         << ", xRange: 0 to " << xRange << endl;

    // do voting in parameter space
    // iterate over every [y][x] inside dataIn
    for(int y = 0; y < dataNumCols; y++) {
      for(int x = 0; x < dataNumRows; x++) {

        // only do voting if the (x,y) location corresponds to an edgepoint
        if(dataIn[y][x] == maxGreyValue) {

//          cout << "DEBUG - performing voting for dataIn[" << y << "][" << x << "]." << endl;

          // iterate over every possible theta for every possible radius length
          for(int r = minRadius; r <= maxRadius; r++) {
            for(int theta = 0; theta <= 360; theta++) {
              rads = theta * PI / 180;

              // where (a,b) represent (x,y) of the center of a circle
              a = int( x - (r * cos(rads)));
              b = int( y - (r * sin(rads)));
              rI = r - minRadius;

              // transform (a,b) from "2D-graph" space to "array" space
              yI = (b < yMid) ? b : b + yMid;
              xI = (a < xMid) ? a : a + xMid;

//              cout << "DEBUG - rI: " << rI << ", yI: " << yI << ", xI: " << xI << endl;

              // error checking for out of range votes
              if( xI >= 0 && xI < xRange && yI >= 0 && yI < yRange ) {
                accumulator[rI][yI][xI]++;
              }

            }
          }

        }
      }
    }

    if(HT_DEBUG) cout << "DEBUG - finished voting" << endl;

    // find maximum value in parameter space
    for(int r = 0; r < radiusRange; r++) {
      for(int y = 0; y < yRange; y++) {
        for(int x = 0; x < xRange; x++) {
          if (accumulator[r][y][x] > maxVotes)
            maxVotes = accumulator[r][y][x];
        }
      }
    }


    if(HT_DEBUG) cout << "DEBUG: maxVotes: " << maxVotes << endl;

    // using relative thresholding, any cell with more votes than the cutoff is
    // considered a "passing" circle and will be added to the list of circles outputted
    cutoff = int(maxVotes * percentOfMaximum);

    // perform initial voting
    circles = doVoting(accumulator, minRadius, radiusRange, xRange, yRange, cutoff);

    // thin out regions of cells with passing votes


      // todo - perform horizontal and vertical thinning, do each a number of times depending on
      // todo - how many circles were initially found compared to the size of the 3D-array space


    // perform final voting
    circles = doVoting(accumulator, minRadius, radiusRange, xRange, yRange, cutoff);

    if(HT_DEBUG) cout << "DEBUG: completed hough transform." << endl;

    // return [r,y,x] of cells that represent circles (after voting and thinning)
    return circles;
  }


  vector<Circle> doVoting(int ***data3D, int minRadius, int radiusRange, int xRange, int yRange, int cutoff)
  {
    // var initialization
    Circle circle;
    vector<Circle> circles;

    // check all cells, record the coordinates of all all "passing" cells in
    // the collection of circles that is returned to the caller of this function
    for(int r = 0; r < radiusRange; r++) {
      for(int y = 0; y < yRange; y++) {
        for(int x = 0; x < xRange; x++) {

          // relative thresholding
          if( data3D[r][y][x] > cutoff) {
            circle.r = r + minRadius;
            circle.y = y;
            circle.x = x;
            circles.push_back(circle);
          }

        }
      }
    }

    return circles;
  }

  void doHorizontalThinning(int ***data2D, int xRange, int yRange, int cutoff)
  {
    // var initialization
    int voteAdjustmentPerNeighbor = 0.25;
    int passingNeighbors;

    for(int y = 0; y < yRange; y++) {
      for(int x = 0; x < xRange; x++) {

        passingNeighbors = 0;

        // check each neighbor cell, tally how many neighbor cells
        // contain more votes than the cutoff number of votes

        // neighbor above
        if( y - 1 > 0 ) {
          if( (*data2D)[y-1][x] > cutoff)
            passingNeighbors++;
        }

        // neighbor below
        if( y + 1 < yRange ) {
          if( (*data2D)[y+1][x] > cutoff)
            passingNeighbors++;
        }

        // neighbor to the left
        if( x - 1 > 0 ) {
          if( (*data2D)[y][x-1] > cutoff)
            passingNeighbors++;
        }

        // neighbor to the right
        if( x + 1 > 0 ) {
          if( (*data2D)[y][x+1] > cutoff)
            passingNeighbors++;
        }


        // adjust the number of votes for the cell at (x,y) based on how
        // many of its neighbors also had passing votes
        // (the goal is to eliminate cells without many "passing" neighbors)
        // ex:              (           0.25          *        3        )  => 0.75
        (*data2D)[y][x] *= (voteAdjustmentPerNeighbor * passingNeighbors);
      }
    }
  }


  void doVerticalThinning(int ****data3D, int radiusRange, int xRange, int yRange, int cutoff)
  {
    // var initialization
    int staticPercentageRetained = 0.7;
    int voteAdjustmentPerNeighbor = 0.15;
    int passingNeighbors;


    for(int r = 0; r < radiusRange; r++) {
      for (int y = 0; y < yRange; y++) {
        for (int x = 0; x < xRange; x++) {

          passingNeighbors = 0;

          // check the (x,y) cell with a radius one greater and one less
          // than this cell and count if either of those two cells contain
          // more votes than the cutoff number of votes

          // neighbor "above"
          if( r + 1 < radiusRange ) {
            if( (*data3D)[r+1][y][x] > cutoff)
              passingNeighbors++;
          }

          // neighbor "below"
          if( r - 1 > 0 ) {
            if( (*data3D)[r-1][y][x] > cutoff)
              passingNeighbors++;
          }


          // adjust the number of votes for the cell at (x,y) based on how
          // many of its neighbors also had passing votes
          // (the goal is to eliminate cells without many "passing" neighbors)
          // ex:                (            0.70          + (           0.15          *        1        )  => 0.85
          (*data3D)[r][y][x] *= ( staticPercentageRetained + (voteAdjustmentPerNeighbor * passingNeighbors) );
        }
      }
    }


  }


}
#endif