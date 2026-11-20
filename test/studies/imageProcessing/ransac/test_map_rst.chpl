
/*****
      test_map_rst.chpl -
      Test bench for the rotation-scale-translation mapping.

      Call:
        test_map_rst

      c 2015-2018 Primordial Machine Vision Systems
*****/


use ransac_rst;
use ip_corner;
use Math;


/* AFFINE: check six affine coefficients in mapping
   RST:    check rotation, scale, translation values
*/
enum verify {
  AFFINE, RST
}


/**** map_seeds test ****/

/***
    test_map_seeds: Self-checking test bench for the map_seeds procedure.
    returns:   true if all tests pass, fail if any fail
***/
proc test_map_seeds() : bool {
  var corners1 : [1..3] corner;         /* corner locations */
  var corners2 : [1..3] corner;         /* other corner locations */
  var mapping : mapinfo;                /* coordinate mapping 1->2 */
  var expmap : mapinfo;                 /* expected mapping */
  var seed1 : 2 * int;                  /* corners indices for first pair */
  var seed2 : 2 * int;                  /* corners indices for second pair */
  var seed3 : 2 * int;                  /* corners indices for third pair */
  var succeed : bool;                   /* if we get a mapping from seeds */
  var passed = true;                    /* overall pass-fail flag */

  writeln("\nTesting map_seeds");

  seed1 = (1, 2);
  seed2 = (2, 3);
  seed3 = (3, 1);

  /* Test 1 - Rotate 45 degrees CW, scale by 2, 2, translate by (200,100). */
  corners2(1).center = (200, 200);
  corners2(2).center = (400, 400);
  corners2(3).center = (200, 800);

  corners1(1).center = ( 177,   35);
  corners1(2).center = ( 247,  247);
  corners1(3).center = (  35,   35);

  expmap.theta = -45 * pi / 180.0;
  expmap.sx = 2.0;
  expmap.sy = 2.0;
  expmap.dx = 200.0;
  expmap.dy = 100.0;
  expmap.sxx =  1.414;
  expmap.sxy = -1.414;
  expmap.syx =  1.414;
  expmap.syy =  1.414;

  succeed = map_seeds(corners1, corners2, seed1, seed2, seed3, mapping);
  if !verify_map(1, succeed, true, verify.RST, mapping, expmap) then
    passed = false;
  if !verify_map(1, succeed, true, verify.AFFINE, mapping, expmap) then
    passed = false;

  /* Test 1 - Rotate 30 degrees CW, scale by 1.4, 1.5, translate by (50,100). */
  corners2(1).center = (350, 500);
  corners2(2).center = (400, 600);
  corners2(3).center = (200, 800);

  corners1(1).center = ( 383,  164);
  corners1(2).center = ( 325,  351);
  corners1(3).center = ( 319,  124);

  expmap.theta = -30 * pi / 180.0;
  expmap.sx = 1.4;
  expmap.sy = 1.5;
  expmap.dx = 50.0;
  expmap.dy = 100.0;
  expmap.sxx =  1.218;
  expmap.sxy = -0.698;
  expmap.syx =  0.754;
  expmap.syy =  1.298;

  succeed = map_seeds(corners1, corners2, seed1, seed2, seed3, mapping);
  if !verify_map(2, succeed, true, verify.RST, mapping, expmap) then
    passed = false;
  if !verify_map(2, succeed, true, verify.AFFINE, mapping, expmap) then
    passed = false;

  if passed then writeln("  all tests passed");
  else writeln("  some test failed");

  return passed;
}

/***
    verify_map:  Test the expected mapping against the actual.
    args:        testno - test case number to print if fail
                 succeed - return value from map_seeds
                 expsucceed - expected value for succeed
                 which - which values in mapinfo to check
                 mapping - transform found by map_seeds
                 expmap - expected transform
    returns:   true if value match, false if not
***/
proc verify_map(testno : int, succeed : bool, expsucceed : bool,
                which : verify, mapping : mapinfo, expmap : mapinfo) : bool {
  var passed = true;                    /* pass/fail flag */

  if (succeed != expsucceed) {
    writef("  test %i expected %s from map_seeds, got %s\n", testno, expsucceed,
           succeed);
    passed = false;
  }

  if (verify.AFFINE == which) {
    if (0.01 < abs(mapping.sxx - expmap.sxx)) {
      writef("  test % expected sxx %.6.3dr, got %.63dr\n", testno,
             mapping.sxx, expmap.sxx);
      passed = false;
    }
    if (0.01 < abs(mapping.sxy - expmap.sxy)) {
      writef("  test % expected sxy %.6.3dr, got %.63dr\n", testno,
             mapping.sxy, expmap.sxy);
      passed = false;
    }
    if (0.01 < abs(mapping.syx - expmap.syx)) {
      writef("  test % expected syx %.6.3dr, got %.63dr\n", testno,
             mapping.syx, expmap.syx);
      passed = false;
    }
    if (0.01 < abs(mapping.syy - expmap.syy)) {
      writef("  test % expected syy %.6.3dr, got %.63dr\n", testno,
             mapping.syy, expmap.syy);
      passed = false;
    }
  } else {
    if ((pi / 180.0) < abs(mapping.theta - expmap.theta)) {
      writef("  test %i expected theta %6.1dr deg, got %6.1dr\n", testno,
             180.0 * expmap.theta / pi, 180.0 * mapping.theta / pi);
      passed = false;
    }
    if (0.1 < (mapping.sx - expmap.sx)) {
      writef("  test %i expected sx %5.3dr, got %5.3dr\n", testno, mapping.sx,
             expmap.sx);
      passed = false;
    }
    if (0.1 < (mapping.sy - expmap.sy)) {
      writef("  test %i expected sy %5.3dr, got %5.3dr\n", testno, mapping.sy,
             expmap.sy);
      passed = false;
    }

  }
  if (1.0 < (mapping.dx - expmap.dx)) {
    writef("  test %i expected dx %6.1dr, got %6.1dr\n", testno, mapping.dx,
           expmap.dx);
    passed = false;
  }
  if (1.0 < (mapping.dy - expmap.dy)) {
    writef("  test %i expected dy %6.1dr, got %6.1dr\n", testno, mapping.dy,
           expmap.dy);
    passed = false;
  }

  return passed;
}


/**** Top Level ****/

proc main() {
  var passed = true;                    /* overall pass-fail flag */

  if !test_map_seeds() then passed = false;

  writeln("\nOverall test results");
  if (passed) then writeln("  all tests passed");
  else writeln("  some test FAILED");

  /* This is needed because main() in ransac_rst returns an int?? */
  return 0;
}
