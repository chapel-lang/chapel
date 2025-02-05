
/*****
      lab_limits_parv1.chpl -
      Determine the actual min/max values for L, A, and B for all possible RGB
      values.  Print the limits out when done.
      This version modifies lab_limits to run the outer loop in parallel.

      Call:
        lab_limits_parv1

      c 2015-2018 Primordial Machine Vision Systems
*****/

use ip_color_v2;
use Time;
use CTypes;

proc main() {
  var lmin, lmax : [0..255] real;         /* limits of L plane values, per r */
  var amin, amax : [0..255] real;         /* limits of A values, per r */
  var bmin, bmax : [0..255] real;         /* limits of B values, per r */
  var lminpos, lmaxpos : [0..255] 3*int;  /* (R,G,B) of L min/max, per r */
  var aminpos, amaxpos : [0..255] 3*int;  /* (R,G,B) of A min/max, per r */
  var bminpos, bmaxpos : [0..255] 3*int;  /* (R,G,B) of B min/max, per r */
  var limit : real;                       /* absolute min/max for L, A, or B */
  var rlim, glim, blim : int;             /* color at limit */
  var x, y, z : real;                     /* CIE XYZ coords */
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
  lmin = max(real);
  lmax = min(real);
  amin = max(real);
  amax = min(real);
  bmin = max(real);
  bmax = min(real);
  forall r in 0..255 {
    for g in 0..255 {
      for b in 0..255 {
        var l, l_a, l_b : real;               /* CIE LAB coords */

        rgbpix_to_lab(r : c_uchar, g : c_uchar, b : c_uchar, l, l_a, l_b);

        if (l < lmin(r)) {
          lmin(r) = l;
          lminpos(r) = (r, g, b);
        }
        if (lmax(r) < l) {
          lmax(r) = l;
          lmaxpos(r) = (r, g, b);
        }

        if (l_a < amin(r)) {
          amin(r) = l_a;
          aminpos(r) = (r, g, b);
        }
        if (amax(r) < l_a) {
          amax(r) = l_a;
          amaxpos(r) = (r, g, b);
        }

        if (l_b < bmin(r)) {
          bmin(r) = l_b;
          bminpos(r) = (r, g, b);
        }
        if (bmax(r) < l_b) {
          bmax(r) = l_b;
          bmaxpos(r) = (r, g, b);
        }
      }
    }
  }

  tpar.stop();

  writef("\nOver RGB cube\n");

  tred.start();
  (limit, (rlim, glim, blim)) = minloc reduce zip(lmin, lminpos);
  writef("  L  %7.2dr (%3i %3i %3i)", limit, rlim, glim, blim);
  (limit, (rlim, glim, blim)) = maxloc reduce zip(lmax, lmaxpos);
  writef("  -  %7.2dr (%3i %3i %3i)\n", limit, rlim, glim, blim);

  (limit, (rlim, glim, blim)) = minloc reduce zip(amin, aminpos);
  writef("  A  %7.2dr (%3i %3i %3i)", limit, rlim, glim, blim);
  (limit, (rlim, glim, blim)) = maxloc reduce zip(amax, amaxpos);
  writef("  -  %7.2dr (%3i %3i %3i)\n", limit, rlim, glim, blim);

  (limit, (rlim, glim, blim)) = minloc reduce zip(bmin, bminpos);
  writef("  B  %7.2dr (%3i %3i %3i)", limit, rlim, glim, blim);
  (limit, (rlim, glim, blim)) = maxloc reduce zip(bmax, bmaxpos);
  writef("  -  %7.2dr (%3i %3i %3i)\n", limit, rlim, glim, blim);

  tred.stop();

  writef("\nTiming [ms]\n");
  writef("  forall    %6.0dr\n", tpar.elapsed() * 1_000);
  writef("  reduce    %6.0dr\n\n", tred.elapsed() * 1_000);
}
