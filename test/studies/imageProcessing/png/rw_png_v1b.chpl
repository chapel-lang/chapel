
/*****
        rw_png_v1b.chpl -
        Program that reads a PNG file from disk, prints the RGB values found
        at one pixel, changes it to 1, 2, 3, and writes the change to disk.
        This version uses the [read|write]_rgb functions to access the image
        data.

        This version embeds the C files needed to compile it, rather than
        using the command line.  See the 'require' statement.

        Call:
          rw_png_v1
            --inname=<file>    file to read from
            --outname=<file>   file to write to
            --x=<#>            x coordinate of pixel to print
            --y=<#>            y coordinate of pixel to print

        c 2015-2018 Primordial Machine Vision Systems
*****/

use CTypes;


require "img_png_v1.h", "build/img_png_v1.o", "-lpng";

/* Command line arguments. */
config const inname : string;           /* name of file to read */
config const outname : string;          /* file to create with modded pixel */
config const x : c_int;                 /* pixel to change */
config const y : c_int;                 /* pixel to change */

/* Our variables. */
var rgb : c_ptr(rgbimage) = nil;        /* the image we read */
var rpix, gpix, bpix : c_uchar;         /* RGB value at x, y */

/* External img_png linkage. */
extern type rgbimage;
extern proc PNG_read(fname : c_ptrConst(c_char), ref img : c_ptr(rgbimage)) : c_int;
extern proc PNG_write(fname : c_ptrConst(c_char), img : c_ptr(rgbimage)) : c_int;
extern proc read_rgb(img : c_ptr(rgbimage), x : c_int, y : c_int,
ref r : c_uchar, ref g : c_uchar, ref b : c_uchar) : c_int;
extern proc write_rgb(img : c_ptr(rgbimage), x : c_int, y : c_int,
r : c_uchar, g : c_uchar, b : c_uchar) : c_int;
extern proc free_rgbimage(ref img : c_ptr(rgbimage)) : void;
/* The rest of the interface we don't use now. */
/*
extern proc PNG_isa(fname : c_string) : c_int;
extern proc alloc_rgbimage(ref img : c_ptr(rgbimage),
ncol : c_int, nrow : c_int) : c_int;
*/


/* Note we're skipping error checking on retval for now. */

PNG_read(inname.c_str(), rgb);
read_rgb(rgb, x,y, rpix, gpix, bpix);
writef("\nAt %4i,%4i   R %3u  G %3u  B %3u\n\n", x,y, rpix,gpix,bpix);

write_rgb(rgb, x,y, 1, 2, 3);
PNG_write(outname.c_str(), rgb);
free_rgbimage(rgb);
