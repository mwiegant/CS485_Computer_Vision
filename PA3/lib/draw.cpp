#ifndef MAX_DRAW
#define MAX_DRAW

#include <cmath>

// todo - add this file to main.cpp  (when I get there)


namespace maxDraw {
  #define PI 3.14159
    /* function prototypes */


  int** drawGreyscaleCircle(int **dataIn, int dataX, int dataY, int centerX, int centerY, int circleR, int color)
  {
    // var init
    int **dataOut;
    int x, y;
    float rads;

    // create blank int** "dataOut"
    dataOut = new int*[dataY];

    for(int i = 0; i < dataY; i++) {
      dataOut[i] = new int[dataX];
    }

    // copy dataIn into dataOut
    for(int j = 0; j < dataY; j++) {
      for(int i = 0; i < dataX; i++) {
        dataOut[j][i] = dataIn[j][i];
      }
    }

    // for all theta from 0 to 360
    for(int theta = 0; theta < 360; theta++) {
      rads = theta * PI / 180;

      // calculate (x,y)
      x = int( centerX + (circleR * cos(rads)) );
      y = int( centerY + (circleR * sin(rads)) );

      // set dataOut[y][x] = color if [y][x] is within bounds
      if( x > 0 && x < dataX && y > 0 && y < dataY )
        dataOut[y][x] = color;
    }


    return dataOut;
  }








}
#endif