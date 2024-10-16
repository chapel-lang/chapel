
/*****
      img_png_v3.c -
      Get and put PNG files from/to disk.  Also includes routines to support
      our in-memory image data structure.

      This version allows saving just a single plane from the image.

      Public Interface:
        PNG_isa - test if file is in PNG format
        PNG_read - read an image from disk
        PNG_write - write an image to disk
        alloc_rgbimage - allocate an image in our format
        free_rgbimage - release an image
        read_rgb - retrieve the value of a pixel
        write_rgb - set the value of a pixel

      c 2015-2018 Primordial Machine Vision Systems, Inc.
*****/

#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <errno.h>
#include <string.h>

#include "img_png_v3.h"


/**** Macros ****/

/***
    RETONERR:  If the previous function call returned an error code (< 0),
               exit the function, returning the code.  Assumes the code has
               been assigned to a local variable 'retval'.
***/
#define RETONERR     { if (retval < 0) { return retval; }}

/***
    CLEANUPONERR:  If the previous function call returned an error code (< 0),
                   jump to the end of the function to clean up any locally
                   allocated storage.  Assumes the error code has been assigned
                   to a local variable 'retval' and that there is a label
                   'cleanup' to jump to.
***/
#define CLEANUPONERR   { if (retval < 0) { goto cleanup; }}



/**** PNG Functions ****/

/***
    PNG_isa:  Read the header of the file and see if it's in PNG format.
    args:     fname - name of file to check
    returns:   true if fname in PNG format, 0 if not
               < 0 on failure (value depends on error)
***/
int PNG_isa(const char *fname) {
  FILE *fin;                            /* file handle to read from */
  png_byte header[8];                   /* PNG file verification */
  char *errmsg;                         /* error message */
  int ispng;                            /* true if PNG file */
  int retval;

  /* For Windows, make this "rb". */
  fin = fopen(fname, "r");
  if (NULL == fin) {
    errmsg = strerror(errno);
    printf("can't open file %s to read: %s\n", fname, errmsg);
    return 0;
  }

  /* Verify is a PNG. */
  retval = fread(&header, 1, 8, fin);
  if (8 != retval) {
    printf("only read %d header bytes from %s\n", retval, fname);
    return 0;
  }

  ispng = !png_sig_cmp(header, 0, 8);

  retval = fclose(fin);
  if (0 != retval) {
    errmsg = strerror(errno);
    printf("problem closing %s: %s\n", fname, errmsg);
    return -1;
  }

  return ispng;
}

/***
    PNG_read:  Bring in a PNG image.  See the libpng manpage for what we're
               doing.  Stored internally as an rgbimage.  Alpha channel is
               ignored.
    args:      fname - name of file with image, if NULL take from stdin
               img - image read (if non-NULL, will free old image)
    returns:   0 if successful
               < 0 on failure (value depends on error)
    modifies:  img
***/
int PNG_read(const char *fname, rgbimage **img) {
  FILE *fin;                            /* file handle to read from */
  png_structp ptr;                      /* internal reference to PNG data */
  png_infop info;                       /* picture information */
  png_bytep *rows;                      /* each row in image */
  png_byte header[8];                   /* PNG file verification */
  char *errmsg;                         /* error message */
  int ispng;                            /* true if PNG file */
  int w, h;                             /* image size */
  int nchan;                            /* number of color channels */
  int x, y, xy;                         /* pixel coordinates/index */
  int i;
  int retval;

  fin = NULL;
  ptr = NULL;
  info = NULL;

  if (NULL == fname) {
    fin = stdin;
  } else {
    /* For Windows, make this "rb". */
    fin = fopen(fname, "r");
    if (NULL == fin) {
      errmsg = strerror(errno);
      printf("can't open file %s to read: %s\n", fname, errmsg);
      return -1;
    }
  }

  /* Verify is a PNG. */
  retval = fread(&header, 1, 8, fin);
  if (8 != retval) {
    printf("only read %d header bytes from %s\n", retval, fname);
    return -1;
  }

  ispng = !png_sig_cmp(header, 0, 8);
  if (!ispng) {
    printf("%s is not in PNG format\n", fname);
    retval = -1;
    goto cleanup;
  }

  ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (NULL == ptr) {
    printf("could not read main PNG structure from %s\n", fname);
    retval = -1;
    goto cleanup;
  }

  info = png_create_info_struct(ptr);
  if (NULL == info) {
    printf("could not read PNG starting info from %s\n", fname);
    retval = -1;
    goto cleanup;
  }

  if (setjmp(png_jmpbuf(ptr))) {
    retval = -1;
    goto cleanup;
  }

  /* Prepare to read */
  png_init_io(ptr, fin);
  png_set_sig_bytes(ptr, 8);

  png_read_png(ptr, info, PNG_TRANSFORM_IDENTITY, NULL);
  rows = png_get_rows(ptr, info);
  h = png_get_image_height(ptr, info);
  w = png_get_image_width(ptr, info);
  nchan = png_get_channels(ptr, info);

  if ((8 != png_get_bit_depth(ptr, info)) ||
      ((2 != png_get_color_type(ptr, info) &&
       (0 != png_get_color_type(ptr,info))))) {
    printf("PNG: unsupported bit depth %d or color type %d\n",
           png_get_bit_depth(ptr, info), png_get_color_type(ptr, info));
    retval = -1;
    goto cleanup;
  }

  retval = alloc_rgbimage(img, w, h);
  CLEANUPONERR;

  if (1 == nchan) {
	  /* Note this correctly reads 1-channel greyscale, where r == g == b. */
    for (y=0, xy=0; y<h; y++) {
      for (x=0; x<w; x++, xy++) {
        (*img)->r[xy] = rows[y][x];
        (*img)->g[xy] = rows[y][x];
        (*img)->b[xy] = rows[y][x];
      }
    }
  } else if ((3 == nchan) || (4 == nchan)) {
    for (y=0, xy=0; y<h; y++) {
      for (x=0, i=0; x<w; x++, xy++, i+=nchan) {
        (*img)->r[xy] = rows[y][i];
        (*img)->g[xy] = rows[y][i+1];
        (*img)->b[xy] = rows[y][i+2];
      }
    }
  } else {
    printf("PNG: do not support %d channels\n", nchan);
    retval = -1;
    goto cleanup;
  }

  retval = 0;

 cleanup:
  if (NULL != fin) {
    retval = fclose(fin);
    if (0 != retval) {
      errmsg = strerror(errno);
      printf("problem closing %s: %s\n", fname, errmsg);
      retval = -1;
    }
  }

  /* Note that png_destroy removes all memory allocated for the image,
     including rows. */
  if (NULL != ptr) {
    if (NULL != info) {
      png_destroy_read_struct(&ptr, &info, NULL);
    } else {
      png_destroy_read_struct(&ptr, NULL, NULL);
    }
  }

  return retval;
}

/***
    PNG_write:  Copy a image to disk.  See the libpng manpage for the flow
                here.  Can save both 8-bit and full-color images.
    args:       fname - name of file to write to, if NULL use stdout
                img - image to save
                plane - which data to store
    returns:   0 if successful
               < 0 on failure (value depends on error)
***/
int PNG_write(const char *fname, rgbimage *img, enum clrplane plane) {
  FILE *fout;                           /* file handle to write to */
  png_structp ptr;                      /* internal reference to PNG data */
  png_infop info;                       /* picture information */
  png_byte *row;                        /* copy of image row to write */
  char *errmsg;                         /* error message */
  int nbyte;                            /* number bytes per pixel */
  int pngtype;                          /* color type for PNG */
  int x, y, xy;                         /* pixel coordinates/index */
  int i;
  int retval;

  fout = NULL;
  row = NULL;

  switch (plane) {
  case CLR_RGB:
    nbyte = 3;
    pngtype = PNG_COLOR_TYPE_RGB;
    break;
  case CLR_GREY:
  case CLR_R:
  case CLR_G:
  case CLR_B:
    nbyte = 1;
    pngtype = PNG_COLOR_TYPE_GRAY;
    break;
  default:
    printf("illegal color plane %d\n", plane);
    return -1;
  }

  if (NULL == fname) {
    fout = stdout;
  } else {
    /* For Windows, "wb". */
    fout = fopen(fname, "w");
    if (NULL == fout) {
      errmsg = strerror(errno);
      printf("can't open file %s to write: %s\n", fname, errmsg);
      return -1;
    }
  }

  if (NULL ==
      (row = (png_byte *) calloc(nbyte * img->ncol, sizeof(png_byte)))) {
    printf("can't allocate local row storage\n");
    retval = -1;
    goto cleanup;
  }

  ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (NULL == ptr) {
    printf("could not prepare PNG for write\n");
    retval = -1;
    goto cleanup;
  }

  info = png_create_info_struct(ptr);
  if (NULL == info) {
    printf("could not prepare PNG info\n");
    retval = -1;
    goto cleanup;
  }

  if (setjmp(png_jmpbuf(ptr))) {
    retval = -1;
    goto cleanup;
  }

  /* Prepare to write. */
  png_init_io(ptr, fout);
  png_set_IHDR(ptr, info, img->ncol, img->nrow, 8, pngtype,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);
  png_write_info(ptr, info);

  for (y=0, xy=0; y<img->nrow; y++) {
    for (x=0, i=0; x<img->ncol; x++, xy++, i+=nbyte) {
      if (CLR_RGB == plane) {
        row[i] = img->r[xy];
        row[i+1] = img->g[xy];
        row[i+2] = img->b[xy];
      } else if ((CLR_GREY == plane) || (CLR_R == plane)) {
        row[i] = img->r[xy];
      } else if (CLR_G == plane) {
        row[i] = img->g[xy];
      } else if (CLR_B == plane) {
        row[i] = img->b[xy];
      } else {
        printf("illegal color plane %d\n", plane);
        retval = -1;
        goto cleanup;
      }
    }
    png_write_row(ptr, row);
  }

  png_write_end(ptr, info);

  retval = 0;

 cleanup:
  if (row) {
    free(row);
  }

  if (NULL != fout) {
    retval = fclose(fout);
    if (0 != retval) {
      errmsg = strerror(errno);
      printf("problem closing %s: %s\n", fname, errmsg);
      retval = -1;
    }
  }

  if (NULL != ptr) {
    if (NULL != info) {
      png_destroy_write_struct(&ptr, &info);
    } else {
      png_destroy_write_struct(&ptr, NULL);
    }
  }


  return retval;
}



/**** rgbimage Support ****/

/***
    alloc_rgbimage:  Reserve memory for our internal storage of a color image.
                     Pixels arrays are zeroed.  The ncol, nrow, and npix
                     fields are filled in.
    args:            img - structure to set up (first freed if non-NULL)
                     ncol, nrow - size of image
    returns:   0 if successful
               < 0 on failure (value depends on error)
    modifies:  img
***/
int alloc_rgbimage(rgbimage **img, int ncol, int nrow) {

  /* The 'free if non-NULL' means we don't try to resize the image, just
     start from scratch. */
  free_rgbimage(img);

  if (NULL == (*img = (rgbimage *) calloc(1, sizeof(rgbimage)))) {
    printf("can't allocate rgb image");
    return -1;
  }

  (*img)->ncol = ncol;
  (*img)->nrow = nrow;
  (*img)->npix = ncol * nrow;

  if (NULL == ((*img)->r = (uchar *) calloc(ncol * nrow, sizeof(uchar)))) {
    printf("can't allocate red plane");
    free(*img);
    *img = NULL;
    return -1;
  }

  if (NULL == ((*img)->g = (uchar *) calloc(ncol * nrow, sizeof(uchar)))) {
    printf("can't allocate green plane");
    free((*img)->r);
    free(*img);
    *img = NULL;
    return - 1;
  }

  if (NULL == ((*img)->b = (uchar *) calloc(ncol * nrow, sizeof(uchar)))) {
    printf("can't allocate blue plane");
    free((*img)->g);
    free((*img)->r);
    free(*img);
    *img = NULL;
    return - 1;
  }

  return 0;
}

/***
    free_rgbimage:  Release the memory stored with an image.
    args:           img - data structure to free
    modifies:  img (set to NULL when done)
***/
void free_rgbimage(rgbimage **img) {

  if (*img) {
    if ((*img)->r) {
      free((*img)->r);
    }
    if ((*img)->g) {
      free((*img)->g);
    }
    if ((*img)->b) {
      free((*img)->b);
    }
    free(*img);
    *img = NULL;
  }
}

/***
    read_rgb:  Get a pixel from the image.
    args:      img - image
               x, y - coordinates of pixel to read
               r, g, b - color planes (all equal if greyscale image)
    returns:   0 if successful
               < 0 on failure (value depends on error)
    modifies:  r, g, b
***/
int read_rgb(rgbimage *img, int x, int y, uchar *r, uchar *g, uchar *b) {
  int xy;                               /* pixel index */

  if ((x < 0) || (y < 0) || (img->ncol <= x) || (img->nrow <= y)) {
    printf("pixel %d,%4d is OOB (image size %d x %4d)\n", x,y,
           img->ncol,img->nrow);
    return -1;
  }

  xy = (y * img->ncol) + x;
  *r = img->r[xy];
  *g = img->g[xy];
  *b = img->b[xy];

  return 0;
}

/***
    write_rgb:  Change a pixel in the image.
    args:       img - image
                x, y - coordinates of pixel to write to
                r, g, b - color planes
    returns:   0 if successful
               < 0 on failure (value depends on error)
    modifies:  img
***/
int write_rgb(rgbimage *img, int x, int y, uchar r, uchar g, uchar b) {
  int xy;                               /* pixel index */

  if ((x < 0) || (y < 0) || (img->ncol <= x) || (img->nrow <= y)) {
    printf("pixel %d,%4d is OOB (image size %d x %4d)\n", x,y,
           img->ncol,img->nrow);
    return -1;
  }

  xy = (y * img->ncol) + x;
  img->r[xy] = r;
  img->g[xy] = g;
  img->b[xy] = b;

  return 0;
}
