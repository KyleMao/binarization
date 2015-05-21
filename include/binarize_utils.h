#include "opencv/cv.h"

int ComputeThreshold(IplImage *img);
unsigned char* extractRaw(IplImage *img, int *width, int *height);
int otsu (unsigned char *image, int rows, int cols, int x0, int y0, int dx, int dy, int vvv);

