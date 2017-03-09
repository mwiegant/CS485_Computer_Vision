#ifndef WRITE_IMAGE
#define WRITE_IMAGE

#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

/*
 * Code for writing to a PGM image.
 *
 * @param fname - the filename of the image
 * @param fimage - a 3D pointer array to hold the image
 * @param M - the width of the image
 * @param N - the height of the image
 * @param Q - the maximum grey value supported by the image
 */
void WriteImage(const char* fname, int **fimage, int M, int N, int Q)
{
 int i, j;
 unsigned char *image;
 ofstream ofp;

 image = (unsigned char *) new unsigned char [M*N];

 // convert the integer values to unsigned char

 for(i=0; i<N; i++)
   for(j=0; j<M; j++)
     image[i*M+j]=(unsigned char)fimage[i][j];

 ofp.open(fname, ios::out);

 if (!ofp) {
   cout << "Can't open file: " << fname << endl;
   exit(1);
 }

 ofp << "P5" << endl;
 ofp << M << " " << N << endl;
 ofp << Q << endl;

 ofp.write( reinterpret_cast<char *>(image), (M*N)*sizeof(unsigned char));

 if (ofp.fail()) {
   cout << "Can't write image " << fname << endl;
   exit(0);
 }

 ofp.close();

}

#endif //WRITE_IMAGE