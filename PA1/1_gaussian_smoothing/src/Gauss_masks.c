#include <math.h>

#define pi 3.141592653589793


/* Create a one-dimensional Gaussian mask
 "mask" of scale "s" (sigma) on "maskSize" pixels.

   The values of the mask are scaled so that the
 sum of mask values = 1.0 
*/
void Gauss_1D_mask(float sigma, int maskSize, float* mask)
{
	int i;
	float cst, tssq, x;
	float sum = 0.0;

	// calculate first part of Gaussian function
	cst = 1.0 / (sigma*sqrt(2.0*pi));

	// Calculate denominator of the e exponential component
	tssq = 1.0 / (2*sigma*sigma);


	// do the remaining math and determine the mask values
	for (i = 0; i < maskSize; i++)
	{
		x = (float) (i-maskSize/2);

		mask[i] = (cst*exp(-(x*x*tssq)));
	}


	// sum up all the mask values so they can be normalized
	for (i = 0; i < maskSize; i++)
	{
		sum += mask[i];
	}


	// normalize the mask values so they end up having a sum of 1.0
	for(i = 0 ; i < maskSize; i++)
	{
		mask[i] /= sum;
	}
}