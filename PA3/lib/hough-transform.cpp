#ifndef MAX_HOUGH_TRANSFORM
#define MAX_HOUGH_TRANSFORM
#define PI 3.14159

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

namespace maxHoughTransform {

  const int HT_DEBUG = true;

  /* structures */
  struct Node {
    int x;
    int y;
    Node* next;
    // constructor
    Node(int _x, int _y, Node* _next) {
      x = _x; y = _y; next = _next;
    }
  };

  struct LinkedList {
    int totalVotes;
    Node* start;
  };

  struct Circle {
    int x;
    int y;
    int r;
    float fittingError;
  };

  /* function prototypes */
  vector<Circle> doVoting(LinkedList ***data3D, int minRadius, int radiusRange, int xRange, int yRange, int cutoff);
  /* both thinning functions want the 2D or 3D array passed by reference (hence *** for 2D and **** for 3D arrays */
  void doHorizontalThinning(LinkedList ***data2D, int xRange, int yRange, int cutoff);
  void doVerticalThinning(LinkedList ****data3D, int radiusRange, int xRange, int yRange, int cutoff);
  void calculateFittingErrors(LinkedList ***data3D, vector<Circle> &circles, int minRadius );


  /* NOTE - call this function from external files! */
  vector<Circle> houghTransform(int **dataIn, int dataNumRows, int dataNumCols, int maxGreyValue, int minRadius, int maxRadius)
  {
    // var initialization
    LinkedList *** accumulator;
    Node* node;
    int radiusRange = maxRadius - minRadius + 1;
    int yRange = dataNumCols * 2;
    int xRange = dataNumRows * 2;
    int yMid = yRange / 2;
    int xMid = xRange / 2;
    int maxVotes = 1;
    int horizontalThinningIterations = 3;
    int verticalThinningIterations = 1;
    int a, b;
    int rI, yI, xI;
    double percentOfMaximum = 0.75;
    int cutoff;
    float rads;
    vector<Circle> circles;

    if(HT_DEBUG) cout << "DEBUG - start of hough transform" << endl;

    // fill accumulator array with 0s
    // dimensions: [0..radiusRange][0..yRange][0..xRange]
    accumulator = new LinkedList**[radiusRange];

    for(int r = 0; r < radiusRange; r++) {

      accumulator[r] = new LinkedList*[yRange];
      for(int y = 0; y < yRange; y++) {

        accumulator[r][y] = new LinkedList[xRange];
        for(int x = 0; x < xRange; x++) {

          // set each element to an empty LinkedList
          accumulator[r][y][x].totalVotes = 0;
          accumulator[r][y][x].start = NULL;
        }
      }
    }

    if(HT_DEBUG) cout << "DEBUG - finished filling accumulator array with 0s" << endl;
    if(HT_DEBUG) cout << "DEBUG - radiusRange: 0 to " << radiusRange << ", yRange: 0 to " << yRange << ", xRange: 0 to " << xRange << endl;

    // do voting in parameter space
    // iterate over every [y][x] inside dataIn
    for(int y = 0; y < dataNumCols; y++) {
      for(int x = 0; x < dataNumRows; x++) {

        // only do voting if the (x,y) location corresponds to an edgepoint
        if(dataIn[y][x] == maxGreyValue) {

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

              // error checking for out of range votes
              if( xI >= 0 && xI < xRange && yI >= 0 && yI < yRange ) {

                // increment number of votes
                accumulator[rI][yI][xI].totalVotes++;

                // append the coordinates of the voter's cell to this cell in
                // the accumulator array
                if(accumulator[rI][yI][xI].start == NULL) {
                  accumulator[rI][yI][xI].start = new Node(a, b, NULL);
                } else {
                  node = accumulator[rI][yI][xI].start;
                  while(node->next != NULL)
                    node = node->next;

                  node->next = new Node(a, b, NULL);
                }
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
          if (accumulator[r][y][x].totalVotes > maxVotes)
            maxVotes = accumulator[r][y][x].totalVotes;
        }
      }
    }


    if(HT_DEBUG) cout << "DEBUG: maxVotes: " << maxVotes << endl;

    // using relative thresholding, any cell with more votes than the cutoff is
    // considered a "passing" circle and will be added to the list of circles outputted
    cutoff = int(maxVotes * percentOfMaximum);

    // perform initial voting
    circles = doVoting(accumulator, minRadius, radiusRange, xRange, yRange, cutoff);

    if(HT_DEBUG) cout << "DEBUG: cutoff: " << cutoff << endl;
    if(HT_DEBUG) cout << "DEBUG: before thinning, found " << circles.size() << " circles." << endl;

    // do horizontal thinning on each radius level
    while(horizontalThinningIterations > 0) {
      for(int r = 0; r < radiusRange; r++) {
        doHorizontalThinning(&accumulator[r], xRange, yRange, cutoff);
      }
      horizontalThinningIterations--;

      // DEBUG - perform voting again so I can see impact of horizontal thinning
      circles = doVoting(accumulator, minRadius, radiusRange, xRange, yRange, cutoff);
      if(HT_DEBUG) cout << "DEBUG: after horizontal thinning, found " << circles.size() << " circles." << endl;
    }


    // do vertical thinning
    while(verticalThinningIterations > 0) {
      doVerticalThinning(&accumulator, radiusRange, xRange, yRange, cutoff);

      verticalThinningIterations--;

      // DEBUG - perform voting again so I can see impact of vertical thinning
      circles = doVoting(accumulator, minRadius, radiusRange, xRange, yRange, cutoff);
      if(HT_DEBUG) cout << "DEBUG: after vertical thinning, found " << circles.size() << " circles." << endl;
    }


    // perform final voting
    circles = doVoting(accumulator, minRadius, radiusRange, xRange, yRange, cutoff);


    // calculate fitting error of circles
//    calculateFittingErrors(accumulator, circles, minRadius);


    if(HT_DEBUG) cout << "DEBUG: completed hough transform." << endl;

    // return [r,y,x] of cells that represent circles (after voting and thinning)
    return circles;
  }


  vector<Circle> doVoting(LinkedList ***data3D, int minRadius, int radiusRange, int xRange, int yRange, int cutoff)
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
          if( data3D[r][y][x].totalVotes > cutoff) {
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

  void doHorizontalThinning(LinkedList ***data2D, int xRange, int yRange, int cutoff)
  {
    // var initialization
    double staticPercentageRetained = 0.00;
    double voteAdjustmentPerNeighbor = 0.25;
    int passingNeighbors;

    for(int y = 0; y < yRange; y++) {
      for(int x = 0; x < xRange; x++) {

        if( (*data2D)[y][x].totalVotes > cutoff) {
          passingNeighbors = 0;

          // check each neighbor cell, tally how many neighbor cells
          // contain more votes than the cutoff number of votes

          // neighbor above
          if( y - 1 > 0 ) {
            if( (*data2D)[y-1][x].totalVotes > cutoff)
              passingNeighbors++;
          }

          // neighbor below
          if( y + 1 < yRange ) {
            if( (*data2D)[y+1][x].totalVotes > cutoff)
              passingNeighbors++;
          }

          // neighbor to the left
          if( x - 1 > 0 ) {
            if( (*data2D)[y][x-1].totalVotes > cutoff)
              passingNeighbors++;
          }

          // neighbor to the right
          if( x + 1 > 0 ) {
            if( (*data2D)[y][x+1].totalVotes > cutoff)
              passingNeighbors++;
          }

          // only adjust the number of votes for cells that are not isolated,
          // meaning none of their neighbors are "passing"
          if(passingNeighbors > 0) {

            // adjust the number of votes for the cell at (x,y) based on how
            // many of its neighbors also had passing votes
            // (the goal is to eliminate cells without many "passing" neighbors)
            // ex:              (           .20           + (           0.20          *        3        )  => 0.80
            (*data2D)[y][x].totalVotes *= ( staticPercentageRetained + (voteAdjustmentPerNeighbor * passingNeighbors) );
          }

        }
      }
    }
  }


  void doVerticalThinning(LinkedList ****data3D, int radiusRange, int xRange, int yRange, int cutoff)
  {
    // var initialization
    double staticPercentageRetained = 0.5;
    double voteAdjustmentPerNeighbor = 0.25;
    int passingNeighbors;


    for(int r = 0; r < radiusRange; r++) {
      for (int y = 0; y < yRange; y++) {
        for (int x = 0; x < xRange; x++) {

          if( (*data3D)[r][y][x].totalVotes > cutoff) {
            passingNeighbors = 0;

            // check the (x,y) cell with a radius one greater and one less
            // than this cell and count if either of those two cells contain
            // more votes than the cutoff number of votes

            // neighbor "above"
            if( r + 1 < radiusRange ) {
              if( (*data3D)[r+1][y][x].totalVotes > cutoff)
                passingNeighbors++;
            }

            // neighbor "below"
            if( r - 1 >= 0 ) {
              if( (*data3D)[r-1][y][x].totalVotes > cutoff)
                passingNeighbors++;
            }

            // only adjust votes on cells that have at least one neighbor
            if(passingNeighbors > 0) {

              // adjust the number of votes for the cell at (x,y) based on how
              // many of its neighbors also had passing votes
              // (the goal is to eliminate cells without many "passing" neighbors)
              // ex:                (            0.70          + (           0.15          *        1        )  => 0.85
              (*data3D)[r][y][x].totalVotes *= ( staticPercentageRetained + (voteAdjustmentPerNeighbor * passingNeighbors) );

            }
          }

        }
      }
    }


  }


  void calculateFittingErrors(LinkedList ***data3D, vector<Circle> &circles, int minRadius )
  {
    // var initialization
    Node* node;
    int r, y, x;
    int xCenter, yCenter;
    int xDiffSquared, yDiffSquared;
    int sum;
    int totalNodes;

    for(int c = 0; c < circles.size(); c++) {
      cout << "DEBUG - checking circle " << c << endl;

      sum = 0;
      r = circles[c].r - minRadius;
      yCenter = circles[c].y;
      xCenter = circles[c].x;

      cout << "data from circle.....r: " << r << ", yCenter: " << yCenter << ", xCenter: " << xCenter << endl;
      cout << "data3D[r][y][x].totalVotes: " << data3D[r][y][x].totalVotes << endl;
      cout << "data3D[r][y][x].start: " << data3D[r][y][x].start << endl;

      totalNodes = data3D[r][y][x].totalVotes;
      node = data3D[r][y][x].start;

      cout << "DEBUG - gathered all data." << endl;

      // calculate how far from the circle each point was that voted for that circle, and add those distances
      while(node != NULL) {

        xDiffSquared = pow( xCenter - node->x, 2);
        yDiffSquared = pow( yCenter - node->y, 2);

        sum += abs( sqrt( xDiffSquared + yDiffSquared) - r );
        node = node->next;
      }

      cout << "calculated sum, sum: " << sum << endl;

      // calculate the fitting error
      circles[c].fittingError = float( sum * 1.0 / totalNodes );
    }
  }

}
#endif