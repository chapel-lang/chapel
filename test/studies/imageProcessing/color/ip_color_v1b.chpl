
/*****
      ip_color.chpl -
      Module holding the common routines and data types for color space
      transformations.  Not a stand-alone program.  'use ip_color_v1b'

      This version uses functions as first-class entities in the rgb_convert
      routing procedure.

      The public bits:
         C Interface
           rgbimage - image with R, G, B planes separated (uchar data)
           CLR_[GREY|RGB|R|G|B] - which plane PNG_write should put in file
                                  (RGB full color, all others 8-bit greys)
           PNG_read - read PNG image from file
           PNG_write - save an rgbimage as a PNG
           PNG_isa - test if a file is a PNG image
           alloc_rgbimage - create a new rgbimage
           free_rgbimage - release the memory of an rgbimage
         Data Structures
           clrspace - HSV | LAB | YUV | RGB
           clrimage - image after color conversion (real data)
           conversion - spec for translating color values back to RGB
             rgbconvert - scaling of color values (reals) to 8-bit
             clrplane - which color to translate
                          C1 = H|L|Y|R  C2 = S|A|U|G  C3 = V(alue)|B|V|B(lue)
         Conversion Functions
           rgb_to_lab - rgbimage to clrimage in CIE L*A*B* coordinates
           rgb_to_hsv - rgbimage to clrimage in HSV coordinates
           rgb_to_yuv - rgbimage to clrimage in YUV coordinates
           rgb_to_rgb - rgbimage to clrimage keeping RGB coordinates
           display_color - clrimage to 8-bit rgbimage RGB coordinates

      c 2015-2018 Primordial Machine Vision Systems
*****/

use CTypes;


/**** C Interfaces ****/

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
extern proc PNG_read(fname : c_string, ref img : c_ptr(rgbimage)) : c_int;
extern proc PNG_write(fname : c_string, img : c_ptr(rgbimage), plane : c_int) : c_int;
extern proc PNG_isa(fname : c_string) : c_int;
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
  var c1 : [area] real;                 /* first color plane (L, H, Y) */
  var c2 : [area] real;                 /* second color plane (A, S, U) */
  var c3 : [area] real;                 /* third color plane (B, V, V) */

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

/* color spaces to which we can convert
     HSV - Hue, Saturation, Value
     LAB - CIE L*A*B*
     YUV - aka. YCbCr
     RGB - RGB, just cast to the internal format
*/
enum clrspace {
  HSV, LAB, YUV, RGB
}

/* how to convert an clrimage plane to an rgbimage
   BOUND - scale uchar range to min/max
   DATA - take limits from data
   CENTER - take limits from data, but if cross zero center at zero
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



/**** Color Conversion - General ****/

/***
    rgb_convert:  Take an RGB image and convert it to some other color
                  space.  An instance of the image in the new space is
                  created.  Basically a routing function to the
                  specific implementations.
    args:         rgb - color image in RGB space
                  clr - converted image
                  space - space to convert to
    modifies:  clr
***/
proc rgb_convert(rgb : c_ptr(rgbimage), ref clr : unmanaged clrimage?, space : clrspace) {
  var clrfn : func(c_uchar, c_uchar, c_uchar, 3*real)?;
                                        /* convert function */
  var xy : int;                         /* pixel index */

  select space {
    when clrspace.LAB do clrfn = rgbpix_to_lab;
    when clrspace.HSV do clrfn = rgbpix_to_hsv;
    when clrspace.YUV do clrfn = rgbpix_to_yuv;
    when clrspace.RGB do clrfn = rgbpix_to_rgb;
    otherwise halt("unknown colorspace ", space);
  }

  clr = new unmanaged clrimage(rgb.deref().ncol, rgb.deref().nrow);

  for (y, x) in clr!.area {
    xy = (y * rgb.deref().ncol) + x;
    (clr!.c1(y,x), clr!.c2(y,x), clr!.c3(y,x)) =
      clrfn!(rgb.deref().r(xy), rgb.deref().g(xy), rgb.deref().b(xy));
  }
}



/**** Color Conversion - LAB ****/

/***
    rgbpix_to_lab:  Convert an RGB pixel into the L*A*B* space.  L range
                    0 - 100, A -500 - 500, B -200 - 200.
    args:        r, g, b - source pixel to convert
    returns:   triple with l, l_a, l_b
***/
inline proc rgbpix_to_lab(r : c_uchar, g : c_uchar, b : c_uchar) : 3*real {
  var l, l_a, l_b : real;               /* color conversion */
  var x, y, z : real;                   /* intermidate conversion to CIE XYZ */

  rgbpix_to_xyz(r, g, b, x, y, z);

  /* 116 = 25 * (100 ** (1/3)) */
  l = 116.0 * lab_map(y / LAB_YNORM) - 16.0;
  l_a = 500.0 * (lab_map(x / LAB_XNORM) - lab_map(y / LAB_YNORM));
  l_b = 200.0 * (lab_map(y / LAB_YNORM) - lab_map(z / LAB_ZNORM));

  return (l, l_a, l_b);
}

/***
    rgbpix_to_xyz:  Convert an RGB pixel into the CIE XYZ space (used for
                    L*A*B*).  XYZ are in the range 0 t/m 1/XYZ_SCALE (5.65075).
    args:           r, g, b - source pixel to convert
                    x, y, z - result
    modifies:  x, y, z
***/
private inline proc rgbpix_to_xyz(r : c_uchar, g : c_uchar, b : c_uchar,
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
private inline proc lab_map(t : real) : real {

  if ((LAB_DELTA ** 3.0) < t) then
    return t ** (1.0 / 3.0);
  else
    return (t / (3.0 * LAB_DELTA * LAB_DELTA)) + (2.0 * LAB_DELTA / 3.0);
}



/**** Color Conversion - HSV ****/

/***
    rgbpix_to_hsv:  Convert an RGB pixel into the HSV space.  H is in degrees
                    (0-360), S and V 0-1.
    args:           r, g, b - source pixel to convert
    returns:   triple with h, s, v
***/
proc rgbpix_to_hsv(r : c_uchar, g : c_uchar, b : c_uchar) : 3*real {
  var h, s, v : real;                   /* color conversion */
  var minrgb : real;                    /* minimum of R, G, B values */
  var maxrgb : real;                    /* maximum of R, G, B values */
  var hsector : int;                    /* which of R, G, B is maximum */

  minrgb = r;
  if (g < minrgb) {
    minrgb = g;
  }
  if (b < minrgb) {
    minrgb = b;
  }

  maxrgb = r;
  hsector = 1;
  if (maxrgb < g) {
    maxrgb = g;
    hsector = 2;
  }
  if (maxrgb < b) {
    maxrgb = b;
    hsector = 3;
  }

  if (maxrgb == minrgb) {
    h = 0.0;
  } else if (1 == hsector) {
    h = 60.0 * (g : real - b) / (maxrgb - minrgb);
  } else if (2 == hsector) {
    h = 120.0 + (60.0 * (b : real - r) / (maxrgb - minrgb));
  } else {
    h = 240.0 + (60.0 * (r : real - g) / (maxrgb - minrgb));
  }
  if (h < 0.0) {
    h += 360.0;
  }
  if (360.0 < h) {
    h -= 360.0;
  }

  if (0.0 == maxrgb) {
    s = 0.0;
  } else {
    s = 1.0 - (minrgb / maxrgb);
  }

  v = maxrgb / 255.0;

  return (h, s, v);
}



/**** Color Conversion - YUV ****/

/***
    rgbpix_to_yuv:  Convert an RGB pixel into the YUV space (aka YCbCr,
                    specifically, ITU-R BT.601).  Y, U, and V are clamped for
                    a direct representation in 8-bit space.  Y offset to range
                    16 t/m 235, U and V 16 t/m 239.
    args:           r, g, b - source pixel to convert
    returns:   triple with y, u, v
***/
proc rgbpix_to_yuv(r : c_uchar, g : c_uchar, b : c_uchar) : 3*real {
  var y, u, v : real;                   /* color conversion */
  var ytmp, utmp, vtmp : real;          /* color values before clamping */

  /* Search fourcc for YUV-RGB conversion; formulas also appear on Wikipedia
     with U = Cb and V = Cr.   Y offset is 16, U, V offset of 128 centers 0
     in uchar range. */

  ytmp = (0.299 * r) + (0.587 * g) + (0.114 * b);
  y = clamp(ytmp + 16.0, 16.0, 235.0);

  utmp = (-0.168736 * r) + (-0.331264 * g) + (0.500 * b);
  u = clamp(utmp + 128.0, 16.0, 239.0);

  vtmp = (0.500 * r) + (-0.418688 * g) + (-0.081312 * b);
  v = clamp(vtmp + 128.0, 16.0, 239.0);

  return (y, u, v);
}

/***
    clamp:  Ensure the value is between the two bounds.  If max < min, value
            is clamped to min.
    args:   val - value to clamp to min/max
            minval - smallest value allowed
            maxval - largest value allowed
    returns:   val in range minval t/m maxval
***/
private inline proc clamp(val : real, minval : real, maxval : real) {

  if (maxval < minval) then return minval;
  else if (val < minval) then return minval;
  else if (maxval < val) then return maxval;
  else return val;
}



/**** Color Conversion - RGB ****/

/***
    rgbpix_to_rgb:  Convert an 8-bit RGB pixel into the type for the Chapel
                    image structure.  A stupid function, but needed so we
                    can use a first-class function in rgb_convert.
    args:           r, g, b - source pixel to convert
    returns:   triple with r, g, b (no modification)
***/
proc rgbpix_to_rgb(r : c_uchar, g : c_uchar, b : c_uchar) : 3*real {

  return (r, g, b);
}



/**** clrimage - rgbimage Conversion ****/

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
    if (rgbconvert.CLIP == spec.how) {
      if (clr(y,x) < 0.0) then pix = 0.0;
      else if (255.0 < clr(y,x)) then pix = 255.0;
      else pix = nearbyint(clr(y,x));
    } else if ((rgbconvert.DATA == spec.how) ||
               (rgbconvert.CENTER == spec.how)) {
      pix = nearbyint(255.0 * (clr(y,x)-minpix) / (maxpix-minpix));
    } else if (rgbconvert.BOUND == spec.how) {
      if (clr(y,x) < spec.min) then pix = 0.0;
      else if (spec.max < clr(y,x)) then pix = 255.0;
      else pix = nearbyint(255.0 * (clr(y,x)-spec.min) / (spec.max-spec.min));
    } else {
      writef("unknown method %s to convert colors to 8-bit\n", spec.how:string);
      return -1;
    }

    rgb(xy) = pix : c_uchar;
  }

  return 0;
}
