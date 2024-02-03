
/*****
      lab_limits_parv2.chpl -
      Determine the actual min/max values for L, A, and B for
      all possible RGB values.  Print the limits out when done.
      This version modifies lab_limits_parv1 to store LAB in 3D arrays and
      do a reduction at the end to find the limits.

      Call:
        lab_limits_parv2

      c 2015-2018 Primordial Machine Vision Systems
*****/

use ip_color_v2;
use Time;
use CTypes;

proc main() {
  const cube : domain(rank=3) = { 0..255, 0..255, 0..255 };
                                        /* space of all possible RGB values */
  var l : [cube] real;                  /* L values per RGB triple */
  var l_a : [cube] real;                /* A values per RGB triple */
  var l_b : [cube] real;                /* B values per RGB triple */
  var limit : real;                     /* absolute min/max for L, A, or B */
  var rlim, glim, blim : int;           /* color at limit */
  var x, y, z : real;                   /* CIE XYZ coords */
  var tpar, tred : stopwatch;

  /* First, verify white.  (255, 255, 255) is our reference white, and
     should give L=100, A=B=0.  Print the intermediate XYZ coords as ref. */
  writeln("\nVerify white point");
  rgbpix_to_xyz(255, 255, 255, x, y, z);
  writef("at (255, 255, 255)   x %7.2dr  y %7.2dr  z %7.2dr\n", x,y,z);
  writef("              norm   x %7.2dr  y %7.2dr  z %7.2dr\n",
         LAB_XNORM, LAB_YNORM, LAB_ZNORM);
  rgbpix_to_lab(255, 255, 255, x, y, z);
  writef("at (255, 255, 255)   l %7.2dr  a %7.2dr  b %7.2dr\n", x,y,z);

  tpar.start();

  /* Now scan the whole RGB space.  We check the min/max along the way so
     we can save the position at the limits.  All variables must be local
     to the loop, or indexed by the forall, to be thread-safe, so we've
     divided the min/max values and RGB triples, calculating each per outer
     loop.  We'll need a reduction at the end over these arrays to find the
     absolute limit.  Note that we could instead save l, a, and b for each
     point in the cube, so just leaving the rgbpix_to_lab call here in the
     loop, and then doing a reduction over the cube.  But that would require
     a lot more storage. */
  forall (r, g, b) in cube do
    rgbpix_to_lab(r : c_uchar, g : c_uchar, b : c_uchar,
                  l(r,g,b), l_a(r,g,b), l_b(r,g,b));

  tpar.stop();

  writef("\nOver RGB cube\n");

  tred.start();

  (limit, (rlim, glim, blim)) = minloc reduce zip(l, cube);
  writef("  L  %7.2dr (%3i %3i %3i)", limit, rlim, glim, blim);
  (limit, (rlim, glim, blim)) = maxloc reduce zip(l, cube);
  writef("  -  %7.2dr (%3i %3i %3i)\n", limit, rlim, glim, blim);

  (limit, (rlim, glim, blim)) = minloc reduce zip(l_a, cube);
  writef("  A  %7.2dr (%3i %3i %3i)", limit, rlim, glim, blim);
  (limit, (rlim, glim, blim)) = maxloc reduce zip(l_a, cube);
  writef("  -  %7.2dr (%3i %3i %3i)\n", limit, rlim, glim, blim);

  (limit, (rlim, glim, blim)) = minloc reduce zip(l_b, cube);
  writef("  B  %7.2dr (%3i %3i %3i)", limit, rlim, glim, blim);
  (limit, (rlim, glim, blim)) = maxloc reduce zip(l_b, cube);
  writef("  -  %7.2dr (%3i %3i %3i)\n", limit, rlim, glim, blim);

  tred.stop();

  writef("\nTiming [ms]\n");
  writef("  forall    %6.0dr\n", tpar.elapsed(TimeUnits.milliseconds));
  writef("  reduce    %6.0dr\n\n", tred.elapsed(TimeUnits.milliseconds));
}
