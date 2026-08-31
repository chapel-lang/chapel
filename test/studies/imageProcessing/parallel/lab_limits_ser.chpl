
/*****
      lab_limits_ser.chpl -
      Exercise 2, determine the actual min/max values for L, A, and B for
      all possible RGB values.  Print the limits out when done.

      Call:
        lab_limits_ser

      c 2015-2018 Primordial Machine Vision Systems
*****/

use ip_color_v2;
use CTypes;

proc main() {
  var l, l_a, l_b : real;               /* CIE LAB coords */
  var lmin, lmax : real;                /* limits of L plane values */
  var amin, amax : real;                /* limits of A values */
  var bmin, bmax : real;                /* limits of B values */
  var lminpos, lmaxpos : 3*int;         /* position of min/max in L */
  var aminpos, amaxpos : 3*int;         /* position of min/max in A */
  var bminpos, bmaxpos : 3*int;         /* position of min/max in B */
  var rpos, gpos, bpos : int;           /* color at min/max position */
  var x, y, z : real;                   /* CIE XYZ coords */

  /* First, verify white.  (255, 255, 255) is our reference white, and
     should give L=100, A=B=0.  Print the intermediate XYZ coords as ref. */
  writeln("\nVerify white point");
  rgbpix_to_xyz(255, 255, 255, x, y, z);
  writef("at (255, 255, 255)   x %7.2dr  y %7.2dr  z %7.2dr\n", x,y,z);
  writef("              norm   x %7.2dr  y %7.2dr  z %7.2dr\n",
         LAB_XNORM, LAB_YNORM, LAB_ZNORM);
  rgbpix_to_lab(255, 255, 255, x, y, z);
  writef("at (255, 255, 255)   l %7.2dr  a %7.2dr  b %7.2dr\n", x,y,z);

  /* Now scan the whole RGB space.  We check the min/max along the way so
     we can save the position at the limits. */
  lmin = max(real);
  lmax = min(real);
  amin = max(real);
  amax = min(real);
  bmin = max(real);
  bmax = min(real);
  for r in 0..255 {
    for g in 0..255 {
      for b in 0..255 {
        rgbpix_to_lab(r : c_uchar, g : c_uchar, b : c_uchar, l, l_a, l_b);

        if (l < lmin) {
          lmin = l;
          lminpos = (r, g, b);
        }
        if (lmax < l) {
          lmax = l;
          lmaxpos = (r, g, b);
        }

        if (l_a < amin) {
          amin = l_a;
          aminpos = (r, g, b);
        }
        if (amax < l_a) {
          amax = l_a;
          amaxpos = (r, g, b);
        }

        if (l_b < bmin) {
          bmin = l_b;
          bminpos = (r, g, b);
        }
        if (bmax < l_b) {
          bmax = l_b;
          bmaxpos = (r, g, b);
        }
      }
    }
  }

  writef("\nOver RGB cube\n");
  (rpos, gpos, bpos) = lminpos;
  writef("  L  %7.2dr (%3i %3i %3i)", lmin, rpos, gpos, bpos);
  (rpos, gpos, bpos) = lmaxpos;
  writef("  -  %7.2dr (%3i %3i %3i)\n", lmax, rpos, gpos, bpos);
  (rpos, gpos, bpos) = aminpos;
  writef("  A  %7.2dr (%3i %3i %3i)", amin, rpos, gpos, bpos);
  (rpos, gpos, bpos) = amaxpos;
  writef("  -  %7.2dr (%3i %3i %3i)\n", amax, rpos, gpos, bpos);
  (rpos, gpos, bpos) = bminpos;
  writef("  B  %7.2dr (%3i %3i %3i)", bmin, rpos, gpos, bpos);
  (rpos, gpos, bpos) = bmaxpos;
  writef("  -  %7.2dr (%3i %3i %3i)\n", bmax, rpos, gpos, bpos);
}
