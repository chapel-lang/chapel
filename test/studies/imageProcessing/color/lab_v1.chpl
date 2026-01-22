
/*****
      lab_v1.chpl -
      Convert an RGB PNG to the L*A*B* color space and save the L (luminance
      or grey-scale) plane to disk.

      Call:
        lab_v1
          --inname=<file>     file to read from
          --outname=<file>    file to write greyscale image to

      c 2015-2018 Primordial Machine Vision Systems
*****/


use Help;
use CTypes;


/**** Declarations ****/

/* Command line arguments */
config var inname : string;             /* name of file to read */
config var outname : string;            /* name of file to write */


/**** C Interface ****/

/* The C image data structure. */
extern record rgbimage {
  var ncol : c_int;                     /* width (columns) of image */
  var nrow : c_int;                     /* height (rows) of image */
  var npix : c_int;                     /* number pixels = w * h */
  var r : c_ptr(c_uchar);               /* red plane */
  var g : c_ptr(c_uchar);               /* green plane */
  var b : c_ptr(c_uchar);               /* blue plane */
}

/* Can't import an enum directly from C; need to grab each component. */
extern const CLR_GREY : int(32);
extern const CLR_RGB : int(32);
extern const CLR_R : int(32);
extern const CLR_G : int(32);
extern const CLR_B : int(32);

/* External img_png linkage. */
extern proc PNG_read(fname : c_ptrConst(c_char), ref img : c_ptr(rgbimage)) : c_int;
extern proc PNG_write(fname : c_ptrConst(c_char), img : c_ptr(rgbimage), plane : c_int) : c_int;
extern proc PNG_isa(fname : c_ptrConst(c_char)) : c_int;
extern proc alloc_rgbimage(ref img : c_ptr(rgbimage),
                           ncol : c_int, nrow : c_int) : c_int;
extern proc free_rgbimage(ref img : c_ptr(rgbimage)) : void;


/**** Constants - Internal ****/

const XYZ_SCALE = 0.17697;              /* scale from CIE XYZ space */
const LAB_DELTA = 6.0 / 29.0;           /* knee point of L*A*B* map function */
const LAB_XNORM = 1.0 / XYZ_SCALE;      /* white references */
const LAB_YNORM = 1.0 / XYZ_SCALE;
const LAB_ZNORM = 1.0 / XYZ_SCALE;
/* The absolute limits of the transform are L 0 t/m 100, A -500 t/m 500,
   B -200 t/m 200.  In practice the RGB space doesn't cover the whole range.
   The limits for A and B are from an exhaustive check of RGB.  Note this
   produces an uncentered point if mapped back to 8-bit, ie. A=0 is at 106
   and B=0 at 128. */
const LAB_LMIN  =    0.0;               /* for scaling from real to uchar */
const LAB_LMAX  =  100.0;
const LAB_AMIN  = -128.15;
const LAB_AMAX  =  182.46;
const LAB_BMIN  = -155.36;
const LAB_BMAX  =  156.20;



/**** Color Conversion - LAB ****/

/***
    rgb_to_lab:  Take an RGB image and convert it the CIE L*A*B* space.
                 Scale the values to fit in a C uchar, putting L in R,
                 A in G, and B in B(lue).
    args:        rgb - color image in RGB space
                 lab - L, A, B stored in R, G, B planes as uchar
    returns:   0 if successful
               < 0 on failure (value depends on error)
    modifies:  lab
***/
proc rgb_to_lab(rgb : c_ptr(rgbimage), ref lab : c_ptr(rgbimage)) : c_int {
  const imgbounds = 0 .. (rgb.deref().npix-1);  /* range covering image */
  var l : [imgbounds] real;             /* L color plane */
  var l_a : [imgbounds] real;           /* A color plane */
  var l_b : [imgbounds] real;           /* B color plane */
  var clamped : real;                   /* LAB clamped to range MIN, MAX */
  var retval : c_int;

  /* We return an error code from rgb_to_lab because this may fail. */
  retval = alloc_rgbimage(lab, rgb.deref().ncol, rgb.deref().nrow);
  if (retval < 0) then return retval;

  /* Since we scale the converted pixel immediately, it's not really
     necessary to keep the results in the three arrays.  We could define
     l, l_a, and l_b as scalars inside the loop (so local).  We took this
     approach to try out arrays, and in preparation for the next version. */
  for xy in imgbounds {
    rgbpix_to_lab(rgb.deref().r(xy), rgb.deref().g(xy), rgb.deref().b(xy), l(xy),
      l_a(xy), l_b(xy));

    clamped = clamp(l(xy), LAB_LMIN, LAB_LMAX);
    lab.deref().r(xy) =
      ((255.0 * (clamped-LAB_LMIN) / (LAB_LMAX-LAB_LMIN)) + 0.5) : c_uchar;

    clamped = clamp(l_a(xy), LAB_AMIN, LAB_AMAX);
    lab.deref().g(xy) =
      ((255.0 * (clamped-LAB_AMIN) / (LAB_AMAX-LAB_AMIN)) + 0.5) : c_uchar;

    clamped = clamp(l_b(xy), LAB_BMIN, LAB_BMAX);
    lab.deref().b(xy) =
      ((255.0 * (clamped-LAB_BMIN) / (LAB_BMAX-LAB_BMIN)) + 0.5) : c_uchar;
  }

  return 0;
}

/***
    rgbpix_to_lab:  Convert an RGB pixel into the L*A*B* space.
    args:           r, g, b - source pixel to convert
                    l, l_a, l_b - result
    modifies:  l, l_a, l_b
***/
inline proc rgbpix_to_lab(r : c_uchar, g : c_uchar, b : c_uchar,
                          out l : real, out l_a : real, out l_b : real) {
  var x, y, z : real;                   /* intermidate conversion to CIE XYZ */

  rgbpix_to_xyz(r, g, b, x, y, z);

  /* 116 = 25 * (100 ** (1/3))
     l will range from -16 to 100, l_a -500 to +500, l_b -200 to +200 */
  l = 116.0 * lab_map(y / LAB_YNORM) - 16.0;
  l_a = 500.0 * (lab_map(x / LAB_XNORM) - lab_map(y / LAB_YNORM));
  l_b = 200.0 * (lab_map(y / LAB_YNORM) - lab_map(z / LAB_ZNORM));
}

/***
    rgbpix_to_xyz:  Convert an RGB pixel into the CIE XYZ space (used for
                    L*A*B*).  XYZ are in the range 0 t/m 1/XYZ_SCALE (5.65075).
    args:           r, g, b - source pixel to convert
                    x, y, z - result
    modifies:  x, y, z
***/
inline proc rgbpix_to_xyz(r : c_uchar, g : c_uchar, b : c_uchar,
                          out  x : real, out y : real, out z : real) {

  /* The original RGB space is 0 t/m 1, so divide by 255.0. */
  x = (0.49 * r    + 0.31 * g    + 0.2 * b)     / (255.0 * XYZ_SCALE);
  y = (0.17697 * r + 0.81240 * g + 0.01063 * b) / (255.0 * XYZ_SCALE);
  z = (              0.01 * g    + 0.99 * b)    / (255.0 * XYZ_SCALE);
}

/***
    lab_map:  L*A*B* scaling function (non-linear mapping function).
    args:     t - value to map
    returns:   re-mapping of t
***/
inline proc lab_map(t : real) : real {

  if ((LAB_DELTA ** 3.0) < t) then
    return t ** (1.0 / 3.0);
  else
    return (t / (3.0 * LAB_DELTA * LAB_DELTA)) + (2.0 * LAB_DELTA / 3.0);
}

/***
    clamp:  Ensure the value is between the two bounds.  If max < min, value
            is clamped to min.
    args:   val - value to clamp to min/max
            minval - smallest value allowed
            maxval - largest value allowed
    returns:   val in range minval t/m maxval
***/
inline proc clamp(val : real, minval : real, maxval : real) {

  if (maxval < minval) then return minval;
  else if (val < minval) then return minval;
  else if (maxval < val) then return maxval;
  else return val;
}



/**** Top Level ****/

/***
    usage:  Print an error message along with the system help, then exit.
    args:   msg - message to print
***/
proc usage(msg : string) {

  writeln("\nERROR");
  writeln("  " + msg);
  printUsage();
  exit(1);
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
    if (inst(i).type == rgbimage) then free_rgbimage(inst(i));
    else if isClass(inst(i)) then delete inst(i);
  }
  exit(1);
}

/***
    verify_setup:  Check that the config variables have been set correctly.
                   If not, print usage error and exit.
***/
proc verify_setup() : void {

  if ("" == inname) then usage("missing --inname");
  if (!PNG_isa(inname.c_str())) then usage("input file not a PNG picture");
  if ("" == outname) then usage("missing --outname");
}


/* Here we go. */

proc main() {
  var rgb : c_ptr(rgbimage);            /* the image we've read */
  var grey : c_ptr(rgbimage);           /* L (greyscale) channel of rgb */
  var retval : int;                     /* return value with error code */

  verify_setup();

  retval = PNG_read(inname.c_str(), rgb);
  end_onerr(retval, rgb);

  retval = rgb_to_lab(rgb, grey);
  end_onerr(retval, rgb, grey);

  retval = PNG_write(outname.c_str(), grey, CLR_R);
  end_onerr(retval, rgb, grey);

  free_rgbimage(rgb);
  free_rgbimage(grey);

  return 0;
}
