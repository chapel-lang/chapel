
/*****
      color_convert_v1b.chpl -
      General purpose converter from the RGB color space to another.  One
      plane of the new space is saved in an 8-bit PNG.

      This version uses the ip_color_v1b library.  The only difference with
      color_convert.chpl is the 'use ip_color_v1b;' line.

      Call:
        color_convert_v1b
          --inname=<file>      PNG file to read
          --outname=<file>     converted color plane
          --plane=C1|C2|C3     which plane to save, # is position in space
                                 ex. C1=L, C2=A, C3=B   (default C1)
          --space=LAB|HSV|YUV  which color space to convert to (default LAB)

      c 2015-2018 Primordial Machine Vision Systems
*****/

use ip_color_v1b;
use Help;
use CTypes;


/**** Command Line Arguments ****/

config const inname : string;                  /* name of file to read */
config const outname : string;                 /* name of file to write */
config const plane : clrplane = clrplane.C1;   /* which plane to write */
config const space : clrspace = clrspace.LAB;  /* convert to which space */


/**** Top Level ****/

/***
    usage:  Print an error message along with the system help, then exit.
    args:   msg - message to print
***/
proc usage(msg : string) {

  writeln("\nERROR");
  writeln("  " + msg);
  writeln("\nALLOWED VALLUES");
  writeln("  --plane = C1 | C2 | C3   (C1 = L, H, Y; C2 = A, S, U; C3 = B, V, V)");
  writeln("  --space = HSV | LAB | YUV");
  printUsage();
  exit(1);
}

/***
    verify_setup:  Check that the config variables have been set correctly.
                   If not, print usage error and exit.
***/
proc verify_setup() {

  if ("" == inname) then usage("missing --inname");
  if (!PNG_isa(inname.c_str())) then usage("input file not a PNG picture");
  if ("" == outname) then usage("missing --outname");
  if (clrplane.ALL == plane) then usage("can only export single plane, use C[123]");
}

/***
    end_onerr:  Check the error code; if OK (>= 0) do nothing.  Else release
                any objects passed as additional arguments - anything can
                be passed and its type will determine the action that needs
                to be done - and exit with an non-zero error value.
    args:       retval - error code/return to value for exit
                inst - variable list of instances to free
***/
proc end_onerr(retval : int, inst ...?narg) : void {

  if (0 <= retval) then return;

  /* Note we skip the argument if we don't know how to clean it up. */
  for param i in 0..(narg-1) {
    if (inst(i).type == c_ptr(rgbimage)) then free_rgbimage(inst(i));
    else if isClass(inst(i)) then delete inst(i);
  }
  exit(1);
}


proc main() {
  var rgb : c_ptr(rgbimage);            /* image we've read */
  var clr : unmanaged clrimage?;        /* converted image */
  var grey : c_ptr(rgbimage);           /* display version of plane */
  var clr2rgb : conversion;             /* how to display L plane */
  var retval : int;                     /* return value with error code */

  verify_setup();

  retval = PNG_read(inname.c_str(), rgb);
  end_onerr(retval, rgb);

  rgb_convert(rgb, clr, space);

  clr2rgb.plane = plane;
  if (clrspace.YUV == space) {
    clr2rgb.how = rgbconvert.CLIP;
  } else if (clrspace.LAB == space) {
    clr2rgb.how = rgbconvert.BOUND;
    if (clrplane.C1 == plane) {
      clr2rgb.min = LAB_LMIN;
      clr2rgb.max = LAB_LMAX;
    } else if (clrplane.C2 == plane) {
      clr2rgb.min = LAB_AMIN;
      clr2rgb.max = LAB_AMAX;
    } else if (clrplane.C3 == plane) {
      clr2rgb.min = LAB_BMIN;
      clr2rgb.max = LAB_BMAX;
    }
  } else if (clrspace.HSV == space) {
    clr2rgb.how = rgbconvert.BOUND;
    if (clrplane.C1 == plane) {
      clr2rgb.min = 0.0;
      clr2rgb.max = 360.0;
    } else {
      clr2rgb.min = 0.0;
      clr2rgb.max = 1.0;
    }
  } else if (clrspace.RGB == space) {
    clr2rgb.how = rgbconvert.CLIP;
  } else {
    /* rgb_to_clrspace will print a warning, just say we're going down. */
    writeln(" ... exiting");
    exit(-1);
  }

  retval = display_color(clr, grey, clr2rgb);
  end_onerr(retval, rgb, grey);

  retval = PNG_write(outname.c_str(), grey, CLR_R);
  end_onerr(retval, rgb, grey);

  free_rgbimage(rgb);
  free_rgbimage(grey);
  delete clr;

  return 0;
}
