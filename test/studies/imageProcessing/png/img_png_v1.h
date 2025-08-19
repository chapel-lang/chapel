
/*****
      img_png_v1.h -
      Public declarations for the PNG file support.  Also includes support
      for our in-memory image data structure.

      Public Interface:
        PNG_isa - test if file is in PNG format
        PNG_read - read an image from disk
        PNG_write - write an image to disk
        alloc_rgbimage - allocate our internal image storage
        free_rgbimage - release image memory
        read_rgb - get a pixel in the image
        write_rgb - set a pixel

      Required Libraries:
        libpng

      c 2015-2018 Primordial Machine Vision Systems, Inc.
*****/

#ifndef _IMGPNG
#define _IMGPNG 1


/*** Data Types ***/

typedef unsigned char uchar;


/*** Data Structures ***/

/* a color image with RGB planes stored separately */
typedef struct _rgbimage {
  int ncol;                             /* width (number columns) of image */
  int nrow;                             /* height (number rows) of image */
  int npix;                             /* number pixels = w * h */
  uchar *r;                             /* red plane */
  uchar *g;                             /* green plane */
  uchar *b;                             /* blue plane */
} rgbimage;


/*** External Functions ***/

/* test if a file is in PNG format, returning true if so, 0 if not
     fname - name of file to read
*/
extern int PNG_isa(const char *);

/* read a PNG image, converting it to an rgbimage
     fname - name of file to read (if NULL, use stdin)
     img - pointer to image to create and read (frees old if non-NULL)
   returns < 0 on error
   modifies img
*/
extern int PNG_read(const char *, rgbimage **);

/* write an rgbimage to disk in PNG format
     fname - name of file to write to (if NULL, use stdout)
     img - image to write
   returns < 0 on error
*/
extern int PNG_write(const char *, rgbimage *);

/* allocate an image in our format, initializing contents to 0
     img - image to create (frees old if non-NULL)
     ncol, nrow - size of image
   returns < 0 on error
   modifies img
*/
extern int alloc_rgbimage(rgbimage **, int, int);

/* release memory for an image
     img - image to free
   modifies img (set to NULL when done)
*/
extern void free_rgbimage(rgbimage **);

/* get pixel's RGB values
     img - image
     x, y - pixel coordinates
     r, g, b - color planes (all modified)
*/
extern int read_rgb(rgbimage *, int, int, uchar *, uchar *, uchar *);

/* change a pixel's RGB values
     img - image
     x, y - pixel coordinates
     r, g, b - color values
*/
extern int write_rgb(rgbimage *, int, int, uchar, uchar, uchar);


#endif   /* _IMGPNG */
