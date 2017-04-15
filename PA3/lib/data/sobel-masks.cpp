#ifndef MAX_DATA_SOBEL_MASKS
#define MAX_DATA_SOBEL_MASKS

/*
 * Given a mask variable, populates the mask with the values
 * of a Sobel Mask in the x direction:
 *
 *  -1  0  1
 *  -2  0  2
 *  -1  0  1
 *
 * Assumes the first dimension is the y-coordinate.
 *
 * Also assumes "mask" has not been initialized yet.
 */

void getSobelMask_X(int** &mask)
{
  // initialize "mask"
  mask = new int*[3];

  for(int i = 0; i < 3; i++)
    mask[i] = new int[3];

//mask[y][x]
  mask[0][0] = -1;
  mask[1][0] = -2;
  mask[2][0] = -1;

  mask[0][1] = 0;
  mask[1][1] = 0;
  mask[2][1] = 0;

  mask[0][2] = 1;
  mask[1][2] = 2;
  mask[2][2] = 1;
}


/*
 * Given a mask variable, populates the mask with the values
 * of a Sobel Mask in the y direction:
 *
 *  -1  -2  -1
 *   0   0   0
 *   1   2   1
 *
 * Assumes the first dimension is the y-coordinate.
 *
 * Also assumes "mask" has not been initialized yet.
 */
void getSobelMask_Y(int** &mask)
{
  // initialize "mask"
  mask = new int*[3];

  for(int i = 0; i < 3; i++)
    mask[i] = new int[3];

//mask[y][x]
  mask[0][0] = -1;
  mask[1][0] = 0;
  mask[2][0] = 1;

  mask[0][1] = -2;
  mask[1][1] = 0;
  mask[2][1] = 2;

  mask[0][2] = -1;
  mask[1][2] = 0;
  mask[2][2] = 1;
}


#endif // MAX_DATA_SOBEL_MASKS