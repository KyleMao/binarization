#include "binarize_utils.h"

int ComputeThreshold(IplImage *img)
{
	int width, height;
	unsigned char *data = extractRaw(img, &width, &height);
	 
	int val = otsu (data, img->height, img->width, 0, 0, img->width, img->height, 1);
	
	delete[] data;

	return val;
	
}


unsigned char* extractRaw(IplImage *img, int *width, int *height)
{
	unsigned char *data;
	unsigned char *temp;

    int step;

    CvSize size;
    int y;
	int x;
	int sz;

    cvGetRawData( img, (uchar**)&temp, &step, &size );
    step /= sizeof(temp[0]);
	
	sz = size.height * size.width;

	data = new unsigned char [sz];
	
	*width = size.width;
	*height = size.height;

	for( y = 0; y < size.height; y++, temp += step)
		for( x = 0; x < size.width; x++ )
		{
			data[y*size.width + x] = temp[x];
		}
	
	
	return data;
}


/*
 the following code was send by Ryan Dibble <dibbler@umich.edu>
 
  The algorithm is very simple but works good hopefully.
 
  Compare the grayscale histogram with a mass density diagram:
  I think the algorithm is a kind of
  divide a body into two parts in a way that the mass 
  centers have the largest distance from each other,
  the function is weighted in a way that same masses have a advantage
  
  TODO:
    RGB: do the same with all colors (CMYG?) seperately 

    test: hardest case = two colors
       bbg: test done, using a two color gray file. Output:
       # OTSU: thresholdValue = 43 gmin=43 gmax=188

 my changes:
   - float -> double
   - debug option added (vvv & 1..2)
   - **image => *image,  &image[i][1] => &image[i*cols+1]

     Joerg.Schulenburg@physik.uni-magdeburg.de

 ToDo:
   - measure contrast
   - detect low-contrast regions

 */

/*======================================================================*/
/* OTSU global thresholding routine                                     */
/*   takes a 2D unsigned char array pointer, number of rows, and        */
/*   number of cols in the array. returns the value of the threshold    */
/*======================================================================*/
int otsu (unsigned char *image, int rows, int cols, int x0, int y0, int dx, int dy, int vvv)
{

  unsigned char *np;    // pointer to position in the image we are working with
  int thresholdValue=1; // value we will threshold at
  int ihist[256];       // image histogram

  int i, j, k;          // various counters
  int n, n1, n2, gmin, gmax;
  double m1, m2, sum, csum, fmax, sb;

  // zero out histogram ...
  memset(ihist, 0, sizeof(ihist));

  gmin=255; gmax=0; k=dy/512+1;
  // generate the histogram
  for (i =  0; i <  dy ; i+=k) {
    np = &image[(y0+i)*cols+x0];
    for (j = 0; j < dx ; j++) {
      ihist[*np]++;
      if(*np > gmax) gmax=*np;
      if(*np < gmin) gmin=*np;
      np++; /* next pixel */
    }
  }

  // set up everything
  sum = csum = 0.0;
  n = 0;

  for (k = 0; k <= 255; k++) {
    sum += (double) k * (double) ihist[k];  /* x*f(x) mass moment */
    n   += ihist[k];                        /*  f(x)    mass      */
  }

  if (!n) {
    // if n has no value we have problems...
    fprintf (stderr, "NOT NORMAL thresholdValue = 160\n");
    return (160);
  }

  // do the otsu global thresholding method

  fmax = -1.0;
  n1 = 0;
  for (k = 0; k < 255; k++) {
    n1 += ihist[k];
    if (!n1) { continue; }
    n2 = n - n1;
    if (n2 == 0) { break; }
    csum += (double) k *ihist[k];
    m1 = csum / n1;
    m2 = (sum - csum) / n2;
    sb = (double) n1 *(double) n2 *(m1 - m2) * (m1 - m2);
    /* bbg: note: can be optimized. */
    if (sb > fmax) {
      fmax = sb;
      thresholdValue = k;
    }
  }

  // at this point we have our thresholding value

  // debug code to display thresholding values
  if ( vvv & 1 )
  //fprintf(stderr,"# OTSU: thresholdValue = %d gmin=%d gmax=%d\n",
    // thresholdValue, gmin, gmax);

  return(thresholdValue);
}

