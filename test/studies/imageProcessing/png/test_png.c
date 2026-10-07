
/*****
      test_png.c -
      Test program for the PNG library.  Reads a PNG file and pixel
      coordinates on the command line and prints the RGB value found
      there, then changes that pixel to 1, 2, 3 and writes the image
      back out.

      Call:
        test_png <PNG image> <output file> <x> <y>

      c 2015-2018 Primordial Machine Vision Systems, Inc.
*****/

#include <stdio.h>
#include <stdlib.h>

#include "img_png_v1.h"


/**** Macros ****/

/***
    CLEANUPONERR:  If the previous function call returned an error code (< 0),
                   jump to the end of the function to clean up any locally
                   allocated storage.  Assumes the error code has been assigned
                   to a local variable 'retval' and that there is a label
                   'cleanup' to jump to.
***/
#define CLEANUPONERR   { if (retval < 0) { goto cleanup; }}

/***
    XYTOIND:  Convert pixel coordinates to an index into the flat image array.
              Evaluates as an expression.
    args:     x, y - coordinates
              ncol - width of image
***/
#define XYTOIND(x, y, ncol)          (((y) * (ncol)) + (x))


/**** Program ****/

/***
    usage:  Print help message and exit.
***/
void usage(void) {

  printf("Usage:  test_png <image> <output (changed pixel)> <x> <y>\n");
  printf("  exiting ...\n");
  exit(1);
}


int main(int argc, char **argv) {
  rgbimage *rgb;                        /* image read from disk */
  uchar r, g, b;                        /* pixel value at x, y */
  int x, y;                             /* pixel coordinates */
  int retval;

  rgb = NULL;

  if (5 != argc) {
    usage();
  }

  if (1 != sscanf(argv[3], "%d", &x)) {
    usage();
  }

  if (1 != sscanf(argv[4], "%d", &y)) {
    usage();
  }

  retval = PNG_read(argv[1], &rgb);
  CLEANUPONERR;

  retval = read_rgb(rgb, x,y, &r, &g, &b);
  CLEANUPONERR;

  printf("\nAt %4d,%4d   R %3d  G %3d  B %3d\n\n", x,y, r,g,b);

  retval = write_rgb(rgb, x,y, 1, 2, 3);
  CLEANUPONERR;

  /* To directly access the pixel, declare an int xy and do this:
  xy = XYTOIND(x, y, rgb->ncol);
  printf("\nAt %4d,%4d   R %3d  G %3d  B %3d\n", x,y, rgb->r[xy], rgb->g[xy],
         rgb->b[xy]);

  rgb->r[xy] = 1;
  rgb->g[xy] = 2;
  rgb->b[xy] = 3;
  */

  retval = PNG_write(argv[2], rgb);
  CLEANUPONERR;

  retval = 0;

 cleanup:
  free_rgbimage(&rgb);

  return retval;
}
