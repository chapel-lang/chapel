
/*****
        rw_png_v4.chpl -
        Program that reads a PNG file from disk, prints the RGB values found
        at one pixel, changes it to 1, 2, 3, and writes the change to disk.
        This version parses the command line for any of the four options
        not passed as flags.

        Call:
          rw_png_v4
            --inname=<file>    file to read from
            --outname=<file>   file to write to
            --x=<#>            x coordinate of pixel to print
            --y=<#>            y coordinate of pixel to print

        c 2015-2018 Primordial Machine Vision Systems
*****/

use Help;
use CTypes;


/* Command line arguments. */
config var inname : string;             /* name of file to read */
config var outname : string;            /* file to create with modded pixel */
config var x : c_int = -1;              /* pixel to change */
config var y : c_int = -1;              /* pixel to change */

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
var rgb : c_ptr(rgbimage);                     /* the image we read */
var xy : int(32);                       /* 1D index of x, y coord */
var retval : c_int;                     /* return value with error code */

/* External img_png linkage. */
extern proc PNG_read(fname : c_ptrConst(c_char), ref img : c_ptr(rgbimage)) : c_int;
extern proc PNG_write(fname : c_ptrConst(c_char), img : c_ptr(rgbimage)) : c_int;
extern proc free_rgbimage(ref img : c_ptr(rgbimage)) : void;
extern proc PNG_isa(fname : c_ptrConst(c_char)) : c_int;
/* The rest of the interface we don't use now. */
/*
extern proc alloc_rgbimage(ref img : rgbimage,
                           ncol : c_int, nrow : c_int) : c_int;
extern proc read_rgb(img : rgbimage, x, y : c_int,
                     ref r, ref g, ref b : c_uchar) : c_int;
extern proc write_rgb(img : rgbimage, x, y : c_int, r, g, b : c_uchar) : c_int;
*/



/**** Top Level ****/

/***
    usage - Print an error message along with the system help, then exit.
    args:   msg - message to print
***/
proc usage(msg : string) {

  writeln("\nERROR");
  writeln("  ", msg);
  printUsage();
  exit(1);
}


proc main(args : [] string) {

  /* Process the command line.  Variables that haven't been specified by
     command-line options will be read in positional order, inname outname x y.
  */
  for a in args[1..] {
    if (("-h" == a) || ("--help" == a)) {
      printUsage();
      exit(0);
    }

    if ("" == inname) then inname = a;
    else if ("" == outname) then outname = a;
    else if (x < 0) then x = a : c_int;
    else if (y < 0) then y = a : c_int;
    else usage("too many arguments on command line");
  }

  /* First sanity check the arguments, then read the image, get the pixel
     requested, change it, and write it back out.  Finally we need to free
     the allocation made in PNG_read. */

  if (x < 0) then
    usage("missing --x or value < 0");
  if (y < 0) then
    usage("missing --y or value < 0");
  if ("" == inname) then
    usage("missing --inname");
  if (!PNG_isa(inname.c_str())) then
    usage("input file not a PNG picture");
  if ("" == outname) then
    usage("missing --outname");

  retval = PNG_read(inname.c_str(), rgb);
  if (retval < 0) {
    free_rgbimage(rgb);
    halt("program stopped after error");
  }

  if (rgb.deref().ncol <= x) {
    free_rgbimage(rgb);
    usage("--x (0-based) >= image width");
  }
  if (rgb.deref().nrow <= y) {
    free_rgbimage(rgb);
    usage("--y (0-based) >= image height");
  }

  /* Now we can access the fields directly. */
  xy = (y * rgb.deref().ncol) + x;
  writef("\nRead %4i x %4i PNG image\n", rgb.deref().ncol, rgb.deref().nrow);
  writef("At %4i,%4i      R %3u  G %3u  B %3u\n\n", x,y,
         rgb.deref().r(xy), rgb.deref().g(xy), rgb.deref().b(xy));

  rgb.deref().r(xy) = 1;
  rgb.deref().g(xy) = 2;
  rgb.deref().b(xy) = 3;

  retval = PNG_write(outname.c_str(), rgb);
  if (retval < 0) {
    free_rgbimage(rgb);
    halt("program stopped after error");
  }

  free_rgbimage(rgb);
}
