
/*****
      lab_v2.chpl -
      Convert an RGB PNG to the L*A*B* color space and save the L (luminance
      or grey-scale) plane to disk.  This version pulls the intermediate
      arrays from rgb_to_lab into a separate data structure, clrimage, and
      makes the conversion back to RGB in a separate proc.

      Call:
        lab_v2
          --inname=<file>     file to read from
          --outname=<file>    file to write greyscale image to

      c 2015-2018 Primordial Machine Vision Systems
*****/


use Help;
use CTypes;
use Math;


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


/**** Data Structures ****/

class clrimage {
  var ncol : int;                       /* width (columns) of image */
  var nrow : int;                       /* height (rows) of image */
  var npix : int;                       /* number pixels = w * h */
  var area : domain(rank=2);            /* array bounds by x and y */
  var c1 : [area] real;                 /* first color plane (L, H, Y, R) */
  var c2 : [area] real;                 /* second color plane (A, S, U, G) */
  var c3 : [area] real;                 /* third color plane (B, V, V, B) */

  /***
      init:      Initializer that allocates the arrays to store the color
                 data.
      args:      w, h - number of columns, rows in image
      modifies:  ncol, nrow, npix, area
  ***/
  proc init(w : int, h : int) {
    ncol = w;
    nrow = h;
    npix = w * h;
    /* This automatically resizes the arrays. */
    area = {0..nrow-1, 0..ncol-1};
  }
}

/* how to convert an clrimage plane to an rgbimage
   BOUND - scale uchar range to min/max
   DATA - take limits from data
   CENTER - limits are +/- largest absolute value in data
   CLIP - clamp values outside min/max range, no scaling
*/
enum rgbconvert {
  BOUND, DATA, CENTER, CLIP
}

/* which plane to handle in clrimage
   C1, C2, C3 - c[123] plane converted, stored in R, G, B (ie. greyscale)
   ALL - c1 converted, stored in R, c2 in G, c3 in B
 */
enum clrplane {
  C1, C2, C3, ALL
}

record conversion {
  var plane : clrplane;                 /* which plane to copy to RGB */
  var how : rgbconvert;                 /* how to convert clr -> rgb */
  var min : real;                       /* lower limit of bound/clip range */
  var max : real;                       /* upper limit of bound/clip range */
}



/**** Color Conversion - LAB ****/

/***
    rgb_to_lab:  Take an RGB image and convert it to the CIE L*A*B* space.
                 An instance of the LAB image is created here.
    args:        rgb - color image in RGB space
                 lab - converted image
    modifies:  lab
***/
proc rgb_to_lab(rgb : c_ptr(rgbimage), ref lab : unmanaged clrimage?) {

  lab = new unmanaged clrimage(rgb.deref().ncol, rgb.deref().nrow);

  for (y, x) in lab!.area {
    const xy = (y * rgb.deref().ncol) + x;      /* pixel index */
    rgbpix_to_lab(rgb.deref().r(xy), rgb.deref().g(xy), rgb.deref().b(xy),
                  lab!.c1(y,x), lab!.c2(y,x), lab!.c3(y,x));
  }
}

/***
    rgbpix_to_lab:  Convert an RGB pixel into the L*A*B* space.
    args:        r, g, b - source pixel to convert
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
                    L*A*B*). XYZ are in the range 0 t/m 1/XYZ_SCALE (5.65075).
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
    display_color:  Convert one or all planes into an 8-bit image (grey or
                    color).  Supports three transforms: scale fixed range to
                    0 t/m 255, no scaling while clipping to limits, or
                    scale data range to 0 t/m 255.
    args:           clr - color space data
                    rgb - displayable RGB image
                    spec - how to map color data to RGB
    returns:   0 if successful
               < 0 on failure (value depends on error)
    modifies:  rgb
***/
proc display_color(clr : unmanaged clrimage?, ref rgb : c_ptr(rgbimage),
                   spec : conversion) : int {
  var xy : int;                         /* pixel index */
  var retval : int;

  retval = alloc_rgbimage(rgb, clr!.ncol : c_int, clr!.nrow : c_int);
  if (retval < 0) then return retval;

  if ((clrplane.C1 == spec.plane) || (clrplane.ALL == spec.plane)) {
    retval = display_plane(clr!.c1, rgb.deref().r, rgb.deref().ncol, spec);
    if (retval < 0) then return retval;

    if (clrplane.C1 == spec.plane) {
      for xy in 0..(rgb.deref().npix-1) {
        rgb.deref().g(xy) = rgb.deref().r(xy);
        rgb.deref().b(xy) = rgb.deref().r(xy);
      }
      return 0;
    }
  }

  if ((clrplane.C2 == spec.plane) || (clrplane.ALL == spec.plane)) {
    retval = display_plane(clr!.c2, rgb.deref().g, rgb.deref().ncol, spec);
    if (retval < 0) then return retval;

    if (clrplane.C2 == spec.plane) {
      for xy in 0..(rgb.deref().npix-1) {
        rgb.deref().r(xy) = rgb.deref().g(xy);
        rgb.deref().b(xy) = rgb.deref().g(xy);
      }
      return 0;
    }
  }

  if ((clrplane.C3 == spec.plane) || (clrplane.ALL == spec.plane)) {
    retval = display_plane(clr!.c3, rgb.deref().b, rgb.deref().ncol, spec);
    if (retval < 0) then return retval;

    if (clrplane.C3 == spec.plane) {
      for xy in 0..(rgb.deref().npix-1) {
        rgb.deref().r(xy) = rgb.deref().b(xy);
        rgb.deref().g(xy) = rgb.deref().b(xy);
      }
      return 0;
    }
  }

  return 0;
}

/***
    display_plane:  Prepare a single color plane for display following the
                    conversion spec.  Supports three transforms: scale fixed
                    range to uchar, no scaling while clipping to uchar, scale
                    data min/max to uchar.
    args:           clr - color plane data (source)
                    rgb - RGB plane (dest)
                    ncol - width of RGB image
                    spec - how to map color data to rgb
    returns:   0 if successful
               < 0 on failure (value depends on error)
    modifies:  rgb
***/
proc display_plane(clr : [] real, rgb : c_ptr(c_uchar), ncol : int,
                   spec : conversion) : c_int {
  var minpix, maxpix : real;            /* data limits */
  var pix : real;                       /* calculated pixel */
  var x, y, xy : int;                   /* pixel coordinates/index */

  if ((rgbconvert.DATA == spec.how) || (rgbconvert.CENTER == spec.how)) {
    minpix = min reduce clr;
    maxpix = max reduce clr;
  }
  if (rgbconvert.CENTER == spec.how) {
    if (abs(minpix) < abs(maxpix)) then minpix = -maxpix;
    else maxpix = -minpix;
  }

  /* Prevent a divide by 0.  Pixel will still map to 0. */
  if (minpix == maxpix) then maxpix = minpix + 1.0;

  for (y, x) in clr.domain {
    xy = (y * ncol) + x;
    select spec.how {
      when rgbconvert.DATA, rgbconvert.CENTER {
        pix = nearbyint(255.0 * (clr(y,x)-minpix) / (maxpix-minpix));
      }
      when rgbconvert.BOUND {
        if (clr(y,x) < spec.min) then pix = 0.0;
        else if (spec.max < clr(y,x)) then pix = 255.0;
        else
          pix = nearbyint(255.0 * (clr(y,x)-spec.min) / (spec.max-spec.min));
      }
      when rgbconvert.CLIP {
        if (clr(y,x) < 0) then pix = 0.0;
        else if (255 < clr(y,x)) then pix = 255.0;
        else pix = nearbyint(clr(y,x));
      }
      otherwise
        /* Sigh.  Can't print an error message that we've changed the enum
           without covering all the cases. */
        return -1;
    }
    rgb(xy) = pix : c_uchar;
  }

  return 0;
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
    verify_setup:  Check that the config variables have been set correctly.
                   If not, print usage error and exit.
***/
proc verify_setup() : void {

  if ("" == inname) then usage("missing --inname");
  if (!PNG_isa(inname.c_str())) then usage("input file not a PNG picture");
  if ("" == outname) then usage("missing --outname");
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
  }
  exit(1);
}


/**** Top Level ****/

proc main() {
  var rgb : c_ptr(rgbimage);            /* the image we've read */
  var lab : unmanaged clrimage?;        /* converted image */
  var grey : c_ptr(rgbimage);           /* displayable version of lab */
  var clr2rgb : conversion;             /* how to display L plane */
  var retval : int;                     /* return value with error code */

  verify_setup();

  retval = PNG_read(inname.c_str(), rgb);
  end_onerr(retval, rgb);

  rgb_to_lab(rgb, lab);

  clr2rgb.plane = clrplane.C1;
  clr2rgb.how = rgbconvert.BOUND;
  clr2rgb.min = LAB_LMIN;
  clr2rgb.max = LAB_LMAX;

  retval = display_color(lab, grey, clr2rgb);
  end_onerr(retval, rgb, grey, lab);

  retval = PNG_write(outname.c_str(), grey, CLR_R);
  end_onerr(retval, rgb, grey, lab);

  free_rgbimage(rgb);
  free_rgbimage(grey);
  delete lab;

  return 0;
}
