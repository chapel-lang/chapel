
/*****
      gabor_v4.chpl -
      Run a Gabor filter over an image.  Calculates the greyscale of the input
      and then runs a filter with the parameters from the command line.  The
      result is scaled to 8-bit and written out as a PNG.
      This version uses a reduction over the kernel after multiplying all
      pixels inside.

      Call:
        gabor_v4
          --inname=<file>          name of file to read
          --outname=<file>         file for results
          --space=[LAB|YUV]        which colorspace for greyscale (L or Y)
          --size=<int>             diameter of kernel, must be odd
          --theta=<real>           rotation of x, y axes, in degrees
          --sclx=<real>            scaling parameter of x axis
          --scly=<real>            scaling parameter of y axis
          --wavelen=<real>         wavelength of sinusoid
          --phi=<real>             phase offset of sinusoid, in degrees

      c 2015-2018 Primordial Machine Vision Systems
*****/


use ip_color_v2;
use Time;
use Help;
use CTypes;
use Math;


config const inname : string;           /* input file name */
config const outname : string;          /* greyscale result of file */
config const space = clrspace.LAB;      /* color space to use for greyscale */
config const size = 13;                 /* kernel size/diameter, odd */
config const theta = 0.0;               /* plane rotation/filter angle, deg */
config const sclx = 2.8;                /* x scaling parameter */
config const scly = 6.0;                /* y scaling parameter */
config const wavelen = 12.0;            /* wavelength of sinusoid */
config const phi = 90.0;                /* offset of sinusoid, deg */



/**** Constants - Internal ****/

/* theta and phi converted to radians */
const theta_rad = pi * wrap_360(theta) / 180.0;
const phi_rad = pi * wrap_360(phi) / 180.0;


/**** Utility Functions ****/

/***
    wrap_360:  Return an angle wrapped to 0.0 to 360.0 (excl.)
    args:      ang - angle (in degrees) to wrap
    returns:   ang in range 0-360
***/
proc wrap_360(in ang : real) {

  while (ang < 0.0) {
    ang += 360.0;
  }
  while (360.0 <= ang) {
    ang -= 360.0;
  }

  return ang;
}


/**** Gabor Filter ****/

/***
    gabor_kernel:  Build a convolution kernel for the Gabor filter.
    args:          theta - angle of rotation, in radians
                   sclx, scly - scaling parameters along axes
                   wavelen - wavelength of sinusoid
                   phi - offset of sinusoid, in radians
                   kernel - pre-allocated size x size array to fill with filter
                            indices symmetric about 0, ie. -size/2..size/2
    modifies:  kernel
***/
proc gabor_kernel(theta : real, sclx : real, scly : real,
                  wavelen : real, phi : real, kernel : [] real) {
  for (y, x) in kernel.domain {
    /* rotated x, y coordinates */
    const xrot = ( x * cos(theta)) + (y * sin(theta));
    const yrot = (-x * sin(theta)) + (y * cos(theta));
    /* square of coordinate after scaling */
    const x2scl = (xrot * xrot) / (sclx * sclx);
    const y2scl = (yrot * yrot) / (scly * scly);

    kernel(y,x) =
      exp(-(x2scl + y2scl) / 2.0) * cos((2.0 * pi * xrot / wavelen) + phi);
  }
}

/***
    run_gaborfilter:  Convolve an image with a Gabor kernel using the
                      parameters specified as arguments.  Source image must
                      be stored in C1, where destination will also be placed.
                      Destination must be as large as source.  Pixels within
                      half a kernel of the edge will be left 0.
    args:             img - image to filter
                      gabor - result of filter
                      size - kernel diameter, must be odd, incl.
                      theta - plane/filter rotation, in radians
                      sclx, scly - scaling parameter of axis
                      wavelen - wavelength of sinusoid
                      phi - offset of sinusoid, in radians
    modifies:  dest
***/
proc run_gaborfilter(img : unmanaged clrimage?, gabor : unmanaged clrimage?, size : int,
                     theta : real, sclx : real, scly : real, wavelen : real,
                     phi : real) {
  const r = (size - 1) / 2;             /* kernel radius */
  const Akern : domain(2) = { -r..r, -r..r };
                                        /* filter matrix */
  const Ainside = img!.area.expand(-r, -r);
                                        /* sub-image to convolve over */
  var kernel : [Akern] real;            /* filter coefficients */
  var prod : [Akern] real;              /* kernel * pixel, per pixel */
  var Aconv : subdomain(img!.area);      /* convolution area of image */

  gabor_kernel(theta=theta, sclx=sclx, scly=scly, wavelen=wavelen,
               phi=phi, kernel=kernel);

  /* Make sure destination is clear. */
  gabor!.c1 = 0.0;

  for (y, x) in Ainside {
    Aconv = img!.area[y-r..y+r, x-r..x+r];
    for ((yc, xc), (kj, ki)) in zip(Aconv, Akern) do
      prod(kj,ki) = img!.c1(yc,xc) * kernel(kj,ki);
    gabor!.c1(y,x) = + reduce prod;
  }
}



/**** Top Level ****/

/***
    usage:  Print an error message along with the system help, then exit.
    args:   msg - message to print
***/
proc usage(msg : string) {

  writeln("\nERROR");
  writeln("  " + msg);
  writeln("\nALLOWED VALLUES");
  writeln("  --space = LAB | YUV");
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
    if (inst(i).type == c_ptr(rgbimage)) then free_rgbimage(inst(i));
    else if isClass(inst(i)) then delete inst(i);
  }
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
  if ((clrspace.LAB != space) && (clrspace.YUV != space))
    then usage("can only use LAB or YUV color spaces");
  if (0 == (size % 2)) then usage("kernel size/diameter must be odd");
}

proc main() {
  var rgb : c_ptr(rgbimage);            /* image we've read */
  var clr : unmanaged clrimage?;        /* converted image with greyscale */
  var grey : c_ptr(rgbimage);           /* display version after filter */
  var gabor : unmanaged clrimage?;      /* result of Gabor filter */
  var gabor2grey : conversion;          /* how to display result */
  var t0, t1, t2, t3, t4, t5 : stopwatch;   /* track time of each fn call */
  var retval : int;

  verify_setup();

  t0.start();

  retval = PNG_read(inname.c_str(), rgb);
  end_onerr(retval, rgb);

  t0.stop(); t1.start();

  rgb_convert(rgb, clr, space);

  t1.stop();

  writef("\nGabor %i x %i   theta %6.1dr  scl %4.2dr %4.2dr   wavelen %4.2dr  phi %6.1dr\n",
         size,size, theta, sclx, scly, wavelen, phi);

  t2.start();

  /* Note this copies a reference of the domain from the old to the new, so
     both clr and gabor share the same. */
  gabor = new unmanaged clrimage(clr);
  run_gaborfilter(clr, gabor, size=size, theta=theta_rad, sclx=sclx, scly=scly,
                  wavelen=wavelen, phi=phi_rad);

  t2.stop(); t3.start();

  gabor2grey.how = rgbconvert.CENTER;
  gabor2grey.plane = clrplane.C1;

  retval = display_color(gabor, grey, gabor2grey);
  end_onerr(retval, rgb, grey, clr, gabor);

  t3.stop(); t4.start();

  retval = PNG_write(outname.c_str(), grey, CLR_R);
  end_onerr(retval, rgb, grey, clr, gabor);

  t4.stop();

  write("\nTiming [ms]\n");
  writef("   PNG_read          %6.0dr\n", t0.elapsed() * 1_000);
  writef("   rgb_convert       %6.0dr\n", t1.elapsed() * 1_000);
  writef("   run_gaborfilter   %6.0dr\n", t2.elapsed() * 1_000);
  writef("   display_color     %6.0dr\n", t3.elapsed() * 1_000);
  writef("   PNG_write         %6.0dr\n", t4.elapsed() * 1_000);

  free_rgbimage(rgb);
  free_rgbimage(grey);
  delete clr;
  delete gabor;

  return 0;
}
