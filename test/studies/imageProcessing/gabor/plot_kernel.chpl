
/*****
      plot_kernel.chpl -
      Create a color image with a heat map of a Gabor function.

      Call:
        plot_kernel
          --outname=<file>         name of file to save heat map in
          --size=<int>             diameter of kernel
          --theta=<real>           rotation of x,y axis, in degrees
          --sclx=<real>            scaling parameter along unrotated x axis
          --scly=<real>            scaling parameter along unrotated y axis
          --wavelen=<real>         wavelength of sinusoid
          --phi=<real>             phase offset of sinusoid, in degrees
          --res=<int>              number of points between kernel points

      c 2015-2018 Primordial Machine Vision Systems
*****/

use CTypes;
use Math;


config const outname = "filter.png";    /* output file name */
config const size = 13;                 /* kernel size */
config const theta = 0.0;               /* plane rotation/filter angle, deg */
config const sclx = 2.8;                /* x scaling parameter */
config const scly = 6.0;                /* y scaling parameter */
config const wavelen = 12.0;            /* wavelength of sinusoid */
config const phi = 90.0;                /* offset of sinusoid, deg */
config const res = 50;                  /* points per element */


/**** Constants ****/

/* Sigh, math.h doesn't implement the M_* constants. */

/* Convert theta and phi to radians */
const theta_rad = pi * theta / 180.0;
const phi_rad = pi * phi / 180.0;


/**** C interface ****/

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

extern proc PNG_write(fname : c_ptrConst(c_char), img : c_ptr(rgbimage), plane : c_int) : c_int;
extern proc PNG_isa(fname : c_ptrConst(c_char)) : c_int;
extern proc alloc_rgbimage(ref img : c_ptr(rgbimage),
                           ncol : c_int, nrow : c_int) : c_int;
extern proc free_rgbimage(ref img : c_ptr(rgbimage)) : void;



/**** Analysis ****/

/***
    gaborfn:  Calculate the Gabor function at a point using the global params.
    args:     x, y - unrotated coordinates
    returns:  Gabor at x, y
***/
inline proc gaborfn(x : real, y : real) : real {
  var xrot, yrot : real;                /* rotated coordinates */
  var x2scl, y2scl : real;              /* square of scaled, rotated coord */

  xrot = x * cos(theta_rad) + y * sin(theta_rad);
  yrot = -x * sin(theta_rad) + y * cos(theta_rad);

  x2scl = (xrot * xrot) / (sclx * sclx);
  y2scl = (yrot * yrot) / (scly * scly);

  return
    exp(-(x2scl + y2scl)/2.0) * cos((2.0 * pi * xrot / wavelen) + phi_rad);
}

/***
    color_encode:  For a value between -1.0 and 1.0, return an RGB color
                   for the heat map.  Heat map goes from black through red
                   and yellow to white
    args:          t - value to convert
                   r, g, b - color representing t
    modifies:  r, g, b
***/
proc color_encode(t : real, out r : c_uchar, out g : c_uchar, out b : c_uchar) {
  var tcomp = (t + 1.0) / 2.0;          /* t in range 0-1.0 */
  /* Formulas taken from gnuplot.  This is the palette 34,35,36.  It goes
     black - red - yellow -white.
        34 = 2t     35 = 2t - 0.5     36 = 2t - 1
     Note that the results of these are clipped at 0 and 1.
  */

  /* gnuplot 34,35,36 */
  if (0.5 < tcomp) {
    r = 255;
  } else {
    r = nearbyint(255.0 * 2.0 * tcomp) : c_uchar;
  }
  if (tcomp < 0.25) {
    g = 0;
  } else if (0.75 < tcomp) {
    g = 255;
  } else {
    g = nearbyint(255.0 * (2.0 * tcomp - 0.5)) : c_uchar;
  }
  if (tcomp < 0.5) {
    b = 0;
    } else {
    b = nearbyint(255.0 * (2.0 * tcomp - 1.0)) : c_uchar;
  }
}



/**** Top Level ****/

proc main() {
  const imgsize = (size + 1) * res;     /* number cols/rows in image */
  const imgscl = 1.0 / res;             /* scale row/col to filter space */
  const imgoff = -((size+1)/2.0);       /* filter space coord of img (0, 0) */
  var heat : c_ptr(rgbimage);                  /* heat map */
  var retval : c_int;

  writef("\nGabor %i x %i   theta %6.1dr  scl %4.2dr %4.2dr   wavelen %4.2dr  phi %6.1dr\n\n",
         size,size, theta, sclx, scly, wavelen, phi);

  retval = alloc_rgbimage(heat, imgsize : c_int, imgsize : c_int);
  if (retval < 0) then exit(retval);

  for row in 0..(imgsize-1) {
    for col in 0..(imgsize-1) {
      const xy = (row * imgsize) + col;
      /* The offset includes half the resolution, so that the filter points
         do not lie on the boundary of the image. */
      const x = imgoff + (col * imgscl);
      /* You might think you'd need to invert y for the plot, but the
         convolutions in gabor_v* are implicitly inverted, so this is correct.
      */
      const y = imgoff + (row * imgscl);
      color_encode(gaborfn(x, y), heat.deref().r(xy), heat.deref().g(xy), heat.deref().b(xy));
    }
  }

  retval = PNG_write(outname.c_str(), heat, CLR_RGB);

  free_rgbimage(heat);

  exit(retval);
}
