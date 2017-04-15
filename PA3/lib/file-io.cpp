#ifndef MAX_FILE_IO
#define MAX_FILE_IO

#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;


/*
 * Code for reading in a PGM image.
 *
 * @param fname - the filename of the image
 * @param fimage - a 3D pointer array to hold the image
 * @param M - the width of the image
 * @param N - the height of the image
 * @param Q - the maximum grey value of the image
 */
void ReadPGMImage(const char* fname, int ***fimage, int& M, int& N, int& Q)
{
  int i, j;
  unsigned char *image;
  char header [100], *ptr;
  ifstream ifp;

  ifp.open(fname, ios::in);

  if (!ifp) {
    cout << "Can't read image: " << fname << endl;
    exit(1);
  }

  // read header

  // verify this image is a PGM image
  ifp.getline(header,100,'\n');
  if ( (header[0]!=80) ||    /* 'P' */
       (header[1]!=53) ) {   /* '5' */
    cout << "Image " << fname << " is not PGM" << endl;
    exit(1);
  }

  ifp.getline(header,100,'\n');
  while(header[0]=='#')
    ifp.getline(header,100,'\n');

  M=strtol(header,&ptr,0);
  N=atoi(ptr);

  ifp.getline(header,100,'\n');

  Q=strtol(header,&ptr,0);

  image = (unsigned char *) new unsigned char [M*N];

  *fimage = new int* [N];
  for(i=0; i<N; i++)
    (*fimage)[i] = new int[M];

  ifp.read( reinterpret_cast<char *>(image), (M*N)*sizeof(unsigned char));

  if (ifp.fail()) {
    cout << "Image " << fname << " has wrong size" << endl;
    exit(1);
  }

  ifp.close();

  //
  // Convert the unsigned characters to integers
  //

  for(i=0; i<N; i++)
    for(j=0; j<M; j++)
      (*fimage)[i][j]=(int)image[i*M+j];
}


/*
 * Code for writing to a PGM image.
 *
 * @param fname - the filename of the image
 * @param fimage - a 3D pointer array to hold the image
 * @param M - the width of the image
 * @param N - the height of the image
 * @param Q - the maximum grey value supported by the image
 */
void WritePGMImage(const char* fname, int **fimage, int M, int N, int Q)
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


#endif //READ_IMAGE