
/*****
      gaborbank.chpl -
      Run a series of Gabor filters over an image, stepping the rotation
      between each.  The filters are run in parallel but are not themselves
      parallel.  Based off the gabor_v1 program.  The results are scaled to
      8-bit and written out as PNGs.  The output name is fixed,
      "bank_<rot>.png" where rot is in degrees.  You can specify the size of
      the bank, the number of rotations between 0 and 180 degrees, on the
      command line.  (180-360 degrees inverts the result.)

      Call:
        gaborbank
          --inname=<file>          name of file to read
          --space=[LAB|YUV]        which colorspace for greyscale (L or Y)
          --nrot=<int>             bank size, must divide 180 evenly
          --size=<int>             diameter of kernel, must be odd
          --sclx=<real>            scaling parameter of x axis
          --scly=<real>            scaling parameter of y axis
          --wavelen=<real>         wavelength of sinusoid
          --phi=<real>             phase offset of sinusoid, in degrees

      c 2015-2018 Primordial Machine Vision Systems
*****/


use ip_color_v2;
use Help;
use CTypes;
use Math;


config const inname : string;           /* input file name */
config const space = clrspace.LAB;      /* color space to use for greyscale */
config const nrot = 18;                 /* number rotations to make */
config const size = 13;                 /* kernel size/diameter, odd */
config const sclx = 2.8;                /* x scaling parameter */
config const scly = 6.0;                /* y scaling parameter */
config const wavelen = 12.0;            /* wavelength of sinusoid */
config const phi = 90.0;                /* offset of sinusoid, deg */



/**** Constants - Internal ****/

/* theta and phi converted to radians */
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
                            centered about 0, ie. -size/2..size/2
    modifies:  kernel
***/
proc gabor_kernel(theta : real, sclx : real, scly : real,
                  wavelen : real, phi : real, ref kernel : [] real) {
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
  const Aconv = img!.area.expand(-r, -r);
                                        /* sub-image to convolve over */
  var kernel : [Akern] real;            /* filter coefficients */

  gabor_kernel(theta=theta, sclx=sclx, scly=scly, wavelen=wavelen,
               phi=phi, kernel=kernel);

  /* Make sure destination is clear. */
  gabor!.c1 = 0.0;

  for (y, x) in Aconv {
    for (kj, ki) in Akern do
      gabor!.c1(y,x) += img!.c1(y+kj,x+ki) * kernel(kj,ki);
  }
}

/***
    filter_and_save:  Run the Gabor filter for a bank and save the result
                      as a PNG.
    args:             img - greyscale image to filter
                      size - diameter of kernel, must be odd
                      theta - angle of rotation, in degrees
                      sclx, scly - scaling parameters for each axis
                      wavelen - wavelength of sinusoid
                      phi_rad - phase offset of sinusoid, in radians
    returns:   0 if successful
               < 0 on failure (value depends on error)
***/
proc filter_and_save(img : unmanaged clrimage?, size : int, theta : int, sclx : real,
                     scly : real, wavelen : real, phi_rad : real) : int {
  const theta_rad = pi * theta / 180;   /* angle of rotation, in radians */
  var gabor : unmanaged clrimage?;      /* result of this filter */
  var grey : c_ptr(rgbimage);           /* display version of result */
  var gabor2grey : conversion;          /* how to display result */
  var outname : string;                 /* output file name */
  var retval : int;

  gabor = new unmanaged clrimage(img);
  run_gaborfilter(img, gabor, size=size, theta=theta_rad, sclx=sclx, scly=scly,
                  wavelen=wavelen, phi=phi_rad);

  gabor2grey.how = rgbconvert.CENTER;
  gabor2grey.plane = clrplane.C1;

  retval = display_color(gabor, grey, gabor2grey);
  if (retval < 0) then return retval;

  /* This makes a 3-digit angle without using something like a sprintf. */
  if (theta < 10) then outname = "bank_00" + theta:string + ".png";
  else if (theta < 100) then outname = "bank_0" + theta:string + ".png";
  else outname = "bank_" + theta:string + ".png";

  retval = PNG_write(outname.c_str(), grey, CLR_R);

  free_rgbimage(grey);
  delete gabor;

  writeln("  done Gabor filter at ", theta, " deg");

  return retval;
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
  if ((clrspace.LAB != space) && (clrspace.YUV != space))
    then usage("can only use LAB or YUV color spaces");
  if (0 == (size % 2)) then usage("kernel size/diameter must be odd");
  if (nrot <= 0) then usage("must have at least one rotation");
  if (0 != (180 % nrot)) then usage("number rotations must divide 180");
}


proc main() {
  var rgb : c_ptr(rgbimage);            /* image we've read */
  var grey : unmanaged clrimage?;       /* greyscale version of RGB */
  var retval : int;

  verify_setup();

  retval = PNG_read(inname.c_str(), rgb);
  end_onerr(retval, rgb);

  rgb_convert(rgb, grey, space);

  coforall bank in 0..nrot-1 {
    const theta = (bank * 180) / nrot;     /* rotation angle in degrees */
    filter_and_save(img=grey, size=size, theta=theta, sclx=sclx,
                    scly=scly, wavelen=wavelen, phi_rad=phi_rad);
  }

  free_rgbimage(rgb);
  delete grey;

  return 0;
}
