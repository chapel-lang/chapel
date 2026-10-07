
/*****
        rw_png_v2.chpl -
        Program that reads a PNG file from disk, prints the RGB values found
        at one pixel, changes it to 1, 2, 3, and writes the change to disk.
        This version accesses the image data directly from Chapel.

        Call:
          rw_png_v2
            --inname=<file>    file to read from
            --outname=<file>   file to write to
            --x=<#>            x coordinate of pixel to print
            --y=<#>            y coordinate of pixel to print

        c 2015-2018 Primordial Machine Vision Systems
*****/

use CTypes;


/* Command line arguments. */
config const inname : string;           /* name of file to read */
config const outname : string;          /* file to create with modded pixel */
config const x : c_int;                 /* pixel to change */
config const y : c_int;                 /* pixel to change */

/* The C image data structure. */
extern record rgbimage {
  var ncol : c_int;                     /* width (columns) of image */
  var nrow : c_int;                     /* height (rows) of image */
  var npix : c_int;                     /* number pixels = w * h */
  var r : c_ptr(c_uchar);               /* red plane */
  var g : c_ptr(c_uchar);               /* green plane */
  var b : c_ptr(c_uchar);               /* blue plane */
}

/* Our variables */
var rgb : c_ptr(rgbimage);              /* the image we read */
var xy : int(32);                       /* 1D index of x, y coord */

/* External img_png linkage. */
extern proc PNG_read(fname : c_ptrConst(c_char), ref img : c_ptr(rgbimage)) : c_int;
extern proc PNG_write(fname : c_ptrConst(c_char), img : c_ptr(rgbimage)) : c_int;
extern proc free_rgbimage(ref img : c_ptr(rgbimage)) : void;
/* The rest of the interface we don't use now. */
/*
extern proc PNG_isa(fname : c_string) : c_int;
extern proc alloc_rgbimage(ref img : rgbimage,
                           ncol : c_int, nrow : c_int) : c_int;
extern proc read_rgb(img : rgbimage, x, y : c_int,
                     ref r, ref g, ref b : c_uchar) : c_int;
extern proc write_rgb(img : rgbimage, x, y : c_int, r, g, b : c_uchar) : c_int;
*/


/**** Top Level ****/

/* Here we go.  First read the image, get the pixel requested, change it, and
   write it back out.  Finally we need to free the allocation made in
   PNG_read. */

PNG_read(inname.c_str(), rgb);

/* Now we can access the fields directly. */
xy = (y * rgb.deref().ncol) + x;
writef("\nRead %4i x %4i PNG image\n", rgb.deref().ncol, rgb.deref().nrow);
writef("At %4i,%4i      R %3u  G %3u  B %3u\n\n", x,y,
       rgb.deref().r(xy), rgb.deref().g(xy), rgb.deref().b(xy));

rgb.deref().r(xy) = 1;
rgb.deref().g(xy) = 2;
rgb.deref().b(xy) = 3;
PNG_write(outname.c_str(), rgb);

free_rgbimage(rgb);
