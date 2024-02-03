
/*****
      ransac_rst.chpl -
      Use the RANSAC method to align to images.  Accounts for rotation,
      scaling, and translation (rst), ie.
           x' = sxx * x + sxy * y + dx
           y' = syx * x + syy * y + dy
      as a general affine transformation where if -sxx * syx == sxy * syy
      we have a rotation followed by a scaling then a translation
           x' =  sx * cos(theta) x + sx * sin(theta) y + dx
           y' = -sy * sin(theta) x + sy * cos(theta) y + dy
      where theta = atan(sxy / sxx), sx = sxx / cos(tehta),
            sy = syy / sin(theta)
      Uses corners from the FAST detector as features.  Prints the best fit
      information when done.

      Call:
        ransac
          --inname1=<file>         first image to compare
          --inname2=<file>         second image to compare
          --outname=<file>         image with matching corners (if provided)
          --space=[LAB|YUV]        color space for greyscale conversion
          --radius=#               radius of FAST detector ring
          --minlen=#               minimum length consecutive series at corner
          --maxlen=#               maximum length consecutive series at corner
          --thr=#                  pixel difference from center of ring
          --suppsep=#              x, y difference to suppress one corner
          --ntry=#                 number random seeds to try
          --nfail=#                number times to re-seed if match fails
          --seedsep=#              min inname2 coord diff b/t seeds
          --mapsep=#               corners after mapping must be this close
          --matchfrac=#            fraction of corners that must match to pass
          --dlen=#                 length diff b/t corners to be same
          --dst=#                  x, y delta in start offsets to be same
          --fixrng=[true|false]    compile param to fix seed for RNG

      c 2015-2018 Primordial Machine Vision Systems
*****/

use ip_color_v3;
use ip_corner;
use kdtree;
use Help;
use Math;
use Random;
use Sort;
use Time;
use CTypes;


/**** Command Line Arguments ****/

/* Input Files */
config const inname1 : string;          /* first image to compare */
config const inname2 : string;          /* second image to compare */
config const outname : string;          /* matching corners image */
/* FAST Detector */
config const space = clrspace.LAB;      /* color space to work in */
config const radius = 3;                /* radius circle for FAST analysis */
config const minlen = 10;               /* shortest FAST run at corner */
config const maxlen = 13;               /* longest consecutive run at corner */
config const thr = 20.0;                /* threshold pixel differing center */
config const suppsep = 5;               /* x, y diff to suppress close corner */
/* RANSAC Alignment */
config const ntry = 250;                /* number random maps to try */
config const nfail = 1500;              /* how often to seed each try */
config const seedsep = 150;             /* min spacing between seeds */
config const mapsep = 5;                /* inlier distance after map */
config const matchfrac = 0.05;          /* min fraction corners must match */
config const dlen = 1;                  /* max delta length differing seq */
config const dst = 3;                   /* max dx dy of start pt around ring */
config param fixrng = false;            /* true to use known seed for RNG */
/* Mapping validity */
param SAME_S_RATIO = 1.25;              /* allowed ratio sx/sy or v.v. */
param MAXSCALE =     3.0;               /* max of sx, 1/sx, sy, or 1/sy */
param SAME_RIGIDITY = 0.05;             /* allowed ratio of affine factors */

const match_margin = 20;                /* pixels between drawn images */
const match_nclr = 20;                  /* number of colors to use for lines */


/**** Data Structures ****/

record mapinfo {
  var sxx, sxy, dx : real;              /* x affine transform */
  var syx, syy, dy : real;              /* y affine transform */
  var theta, sx, sy : real;             /* rotation (rads)/scaling transform */
}

record tryinfo {
  var seed1 : 2*int;                    /* (corner1,corner2) for one seed */
  var seed2 : 2*int;                    /* (corner1,corner2) for second seed */
  var seed3 : 2*int;                    /* (corner1,corner2) for third seed */
  var map : mapinfo;                    /* mapping from seeds */
  var nmap : int;                       /* number of corners mapped */
  var nfail : int;                      /* number failed attempts to seed */
  var ttry : real;                      /* time in ms for try */
}

/**** RANSAC mapping ****/

/***
    align_corners:  Try random pairs of corners to determine the mapping from
                    one image to another.
    args:           corners1 - corners found in first image
                    corners2 - corners found in second image
                    bestmap - best alignment we've found
                    map1to2, map2to1 - corners that match up, by index
    modifies:  bestmap, map1to2, map2to1
***/
proc align_corners(const corners1 : [] corner, const corners2 : [] corner,
                   out bestmap : mapinfo,
                   map1to2 : [] int, map2to1 : [] int) {
  var tries : [1..ntry] tryinfo;        /* details about each try */
  var besttry : int;                    /* try with best result */
  const minlen                          /* smallest number of corners */
    = min(corners1.domain.dim(0).last, corners2.domain.dim(0).last);
  const mincnt                          /* number corners that must match */
    = nearbyint(matchfrac * minlen) : int;
  var tree                              /* 2-dimensional sorted space */
    = new unmanaged kdtree(corners2.domain.size, int, 2);

  for i in corners2.domain {
    tree.add_node(corners2(i).center, i);
  }
  tree.assemble_tree();

  /* Only instantiating one rand outside the loop causes this to execute on
     only one or two cores.  Why is still being investigated.
  */
  forall i in 1..ntry {
    var rand : owned RandomStream(real)?;      /* random numbers to pick seeds */
    var trytime : stopwatch;                /* run time for this try */

    if (fixrng) {
      rand = createRandomStream(real, (2*i) + 1);
    } else {
      rand = createRandomStream(real);
    }

    trytime.start();
    do {
      if pick_seeds(corners1, corners2, rand, tries(i)) {
        var mapped1 : [corners1.domain] corner;
        map_corners(corners1, tries(i).map, mapped1);

        /* We do not try to scale the FAST ring, as the comparison in
           are_corners_similar assumes the rings have the same radius.
           This means that the corners may be somewhat different in the
           two images, but the alternative of scaling the image around each
           corner1 and re-running FAST is too painful. */
        const matchcnt = count_matches(mapped1, corners2, tree);

        if (mincnt <= matchcnt) {
          tries(i).nmap = matchcnt;
          break;
        }
      }
      tries(i).nfail += 1;
    } while (tries(i).nfail < nfail);

    trytime.stop();
    tries(i).ttry = trytime.elapsed() * 1_000;
  }

  besttry = select_besttry(tries);

  /**!!!**/
  /* This is for debug, don't need in production. */
  dump_map("Seed map", tries(besttry).map);

  /* We've avoided storing arrays in the records as it seems to be a problem
     with passing as arguments.  We need to regenerate the mapping for the
     best try because of it (before had stored map[12]to[21] with tryinfo). */
  var mapped1 : [corners1.domain] corner;
  map_corners(corners1, tries(besttry).map, mapped1);
  match_corners(mapped1, corners2, tree, map1to2, map2to1);
  refine_mapping(corners1, corners2, map1to2, map2to1, bestmap);

  dump_tries(tries);

  delete tree;
}

/***
    pick_seeds:  Find three similar corners in each image.  There are no
                 restrictions on their placement relative to each other
                 other than that the mapping they form be valid.  Seeds are
                 stored as (c1, c2) pairs of indices into the corners array
                 and may have garbage if the proc returns false.  Also
                 populates the mapping in the try based on the seeds.
    args:        corners1, corners2 - list of corners in each image
                 rand - random number generator
                 trial - information about this attempt to match
    returns:   true if all three seeds placed, false if cannot find three
    modifies:  trial.seed[123], trial.map
***/
proc pick_seeds(const corners1 : [] corner, const corners2 : [] corner,
                rand : RandomStream?, inout trial : tryinfo) : bool {
  var ind11, ind12 : int;               /* seed1 indices in corners1,2 */
  var ind21, ind22 : int;               /* seed2 indices in corners1,2 */
  var ind31, ind32 : int;               /* seed3 indices in corners1,2 */
  var matches : [corners2.domain] int;  /* possible seeds in corners2 */
  var nmatch : int;                     /* number possible seeds */
  const rng1 = corners1.domain.dim(0);  /* range of indices for corners1 */
  const rng2 = corners2.domain.dim(0);  /* range of indices for corners2 */
  var retval = true;                    /* work-around return value */

  ind11 = random_ranged(rand, rng1);

  nmatch = 0;
  for i in rng2 {
    if are_corners_similar(corners1(ind11), corners2(i)) {
      nmatch += 1;
      matches(nmatch) = i;
    }
  }

  /**!!!**/
  /* Work around a Chapel bug here that double-frees the matches array
     if we were to return directly.  Also for the seed2 and seed3 tests.

     if (0 == nmatch) then return false;
  */
  if (0 == nmatch) {
    retval = false;
  } else {
    ind12 = matches(random_bound(rand, 1, nmatch));
    trial.seed1 = (ind11, ind12);

    do {
      ind21 = random_ranged(rand, rng1);
    } while ((ind11 == ind21) ||
             (seed_distance(corners1, ind11, ind21) <= seedsep));

    nmatch = 0;
    for i in rng2 {
      if ((i != ind12) && (seedsep < seed_distance(corners2, ind12, i))) {
        if are_corners_similar(corners1(ind21), corners2(i)) {
          nmatch += 1;
          matches(nmatch) = i;
        }
      }
    }

    if (0 == nmatch) {
      retval = false;
    } else {
      ind22 = matches(random_bound(rand, 1, nmatch));

      trial.seed2 = (ind21, ind22);

      do {
        ind31 = random_ranged(rand, rng1);
      } while ((ind11 == ind31) || (ind21 == ind31) ||
               (seed_distance(corners1, ind11, ind31) <= seedsep) ||
               (seed_distance(corners1, ind21, ind31) <= seedsep));

      nmatch = 0;
      for i in rng2 {
        var mapping : mapinfo;          /* dummy mapping to validate seed3 */
        if ((i != ind12) && (seedsep < seed_distance(corners2, ind12, i)) &&
            (i != ind22) && (seedsep < seed_distance(corners2, ind22, i)) &&
            are_corners_similar(corners1(ind31), corners2(i))) {
          /* Screen seed3 here to be only those that could satisfy the
             restrictions for scaling and rigidity.  Otherwise there are
             too few hits and we will never find a solution. */
          if map_seeds(corners1, corners2, trial.seed1, trial.seed2, (ind31, i),
                       mapping) {
            nmatch += 1;
            matches(nmatch) = i;
          }
        }
      }

      if (0 == nmatch) {
        retval = false;
      } else {
        var mapping : mapinfo;

        ind32 = matches(random_bound(rand, 1, nmatch));
        trial.seed3 = (ind31, ind32);

        /* Note we know this already passes. */
        map_seeds(corners1, corners2, trial.seed1, trial.seed2, trial.seed3,
                  trial.map);
      }
    }
  }

  return retval;
}

/***
    seed_distance:  Returns the distance between two seeds.  For simplicity
                    we use Manhattan geometry.
    args:           corners - list of corners in image
                    seed1, seed2 - indices in array of seeds to use
    returns:   seed separation
***/
inline proc seed_distance(corners : [] corner, seed1 : int, seed2 : int) : int {
  const (xc, yc)                        /* location of seed1 */
    = corners(seed1).center;
  const (vc, wc)                        /* location of seed2 */
    = corners(seed2).center;

  return abs(xc-vc) + abs(yc-wc);
}

/***
    map_seeds:  Give two seed pairs, determine the mapping from image1 to
                image2 coordinates.
    args:       corners1, corners2 - list of corners in each image
                seed1 - fisrt pair in each corners list to align
                seed2 - second pair in each corners list to align
                seed3 - third pair in each corners list to align
                mapping - transform to get seeds to align
    returns:   true if mapping acceptable, false if no valid mapping
    modifies:  mapping
***/
proc map_seeds(const corners1 : [] corner, const corners2 : [] corner,
               seed1 : 2*int, seed2 : 2*int, seed3 : 2*int,
               inout mapping : mapinfo) : bool {
  /* First [123] is the seed, second [12] is the corners array. */
  const (ind11, ind12) = seed1;         /* seed 1 indices */
  const (ind21, ind22) = seed2;         /* seed 2 indices */
  const (ind31, ind32) = seed3;         /* seed 3 indices */
  var xc1, yc1 : real;                  /* seed 1 corners1 */
  var vc1, wc1 : real;                  /* seed 1 corners2 */
  var xc2, yc2 : real;                  /* seed 2 corners1 */
  var vc2, wc2 : real;                  /* seed 2 corners2 */
  var xc3, yc3 : real;                  /* seed 3 corners1 */
  var vc3, wc3 : real;                  /* seed 3 corners2 */
  /* This seems the easiest way to cast the variables for below. */
  (xc1, yc1) = corners1(ind11).center;
  (vc1, wc1) = corners2(ind12).center;
  (xc2, yc2) = corners1(ind21).center;
  (vc2, wc2) = corners2(ind22).center;
  (xc3, yc3) = corners1(ind31).center;
  (vc3, wc3) = corners2(ind32).center;

  if (0 == xc1) then return false;

  /* Partial factors/terms when solving linear system - see notes for
     algebra. */
  const p2 = xc2 * vc1 / xc1;
  const q2 = yc2 - yc1 * xc2 / xc1;
  const r2 = 1.0 - (xc2 / xc1);
  const p3 = xc3 * vc1 / xc1;
  const q3 = yc3 - yc1 *  (xc3 / xc1);
  const r3 = 1.0 - (xc3 / xc1);

  const s2 = xc2 * wc1 / xc1;
  const s3 = xc3 * wc1 / xc1;

  /* Avoid divide by 0 problems. */
  if ((0.0 == q2) || (0.0 == (r3 - (r2 * q3 / q2)))) then return false;

  mapping.dx = (vc3 - p3 - (q3 / q2) * (vc2 - p2)) / (r3 - (r2 * q3 / q2));
  mapping.dy = (wc3 - s3 - (q3 / q2) * (wc2 - s2)) / (r3 - (r2 * q3 / q2));
  mapping.sxy = (vc2 - p2 - r2 * mapping.dx) / q2;
  mapping.syy = (wc2 - s2 - r2 * mapping.dy) / q2;
  mapping.sxx = (vc1 - (mapping.sxy * yc1) - mapping.dx) / xc1;
  mapping.syx = (wc1 - (mapping.syy * yc1) - mapping.dy) / xc1;

  affine_to_rst(mapping);

  return valid_map(mapping);
}

/***
    affine_to_rst:  Convert the six affine coefficients to a rotation and x
                    and y scale.  This assumes a rigid rotation, which we
                    do not verify.
    args:           mapping - transform to get seeds to align
    modifies:  mapping.theta, mapping.sx, mapping.sy
***/
proc affine_to_rst(inout mapping : mapinfo) {

  mapping.theta = atan2(mapping.sxy, mapping.sxx);
  if (0.0 == cos(mapping.theta)) {
    mapping.sx =  mapping.sxy / sin(mapping.theta);
    mapping.sy = -mapping.syx / sin(mapping.theta);
  } else {
    mapping.sx = mapping.sxx / cos(mapping.theta);
    mapping.sy = mapping.syy / cos(mapping.theta);
  }
}

/***
    valid_map:  Do some sanity checking on the mapping.
    args:       mapping - shift/scale map between image coordinates
    returns:   true if mapping allowed, false if not
***/
proc valid_map(mapping : mapinfo) : bool {

  /* Check for rigidity of rotation.  -sxx * syx = sxy * syy approx. */
  const rigid1 = -mapping.sxx * mapping.syx;
  const rigid2 = mapping.sxy * mapping.syy;
  if (((rigid1 < 0.0) && (0.0 < rigid2)) ||
      ((rigid2 < 0.0) && (0.0 < rigid1)) ||
      (abs(rigid2) < abs((1.0 - SAME_RIGIDITY) * rigid1)) ||
      (abs((1.0 + SAME_RIGIDITY) * rigid1) < abs(rigid2))) {
    return false;
  }

  /* Limit maximum scaling. */
  if ((MAXSCALE < mapping.sx) || (MAXSCALE < (1.0 / mapping.sx)) ||
      (MAXSCALE < mapping.sy) || (MAXSCALE < (1.0 / mapping.sy))) {
    return false;
  }

  /* x and y must scale by roughly the same amount. */
  if ((SAME_S_RATIO < (mapping.sx / mapping.sy)) ||
      (SAME_S_RATIO < (mapping.sy / mapping.sx))) {
    return false;
  }
  /* And they both need to be positive. */
  if ((mapping.sx <= 0.0) || (mapping.sy <= 0.0)) {
    return false;
  }

  /**!!!**/
  /* Other ideas: maximum shift, perhaps as fraction of image size. */

  return true;
}

/***
    map_corners:  Convert the corners in image1 coordinates to image2.
    args:         corners - list of corners to map
                  mapping - x, y scaling and translation
                  mapped - corners1 in corners2 plane
    returns:   mapped
***/
proc map_corners(const corners : [] corner, const in mapping : mapinfo,
                 mapped : [] corner) {

  /* This gets everything; we'll overwrite the center and start. */
  mapped = corners;

  forall c in corners.domain with (const in mapping, ref mapped) {
    const (xc, yc) = corners(c).center;
    const vc
      = nearbyint(mapping.sxx * xc + mapping.sxy * yc + mapping.dx) : int;
    const wc
      = nearbyint(mapping.syx * xc + mapping.syy * yc + mapping.dy) : int;

    mapped(c).center = (vc, wc);
    mapped(c).st(0) += vc - xc;
    mapped(c).st(1) += wc - yc;
  }
}

/***
    count_matches:  Count the number of corners that are similar and close.
    args:           corners1 - corners to match, in corners2 coords
                    corners2 - corners to be matched
                    tree - sorted tree of corners2 points
    returns:   number of corners that match
***/
proc count_matches(const corners1 : [] corner, const corners2 : [] corner,
                   tree : kdtree) : int {
  var map1to2 : [corners1.domain] int;  /* corners2 matching corners1 */
  var map2to1 : [corners2.domain] int;  /* corners1 matching corners2 */

  return match_corners(corners1, corners2, tree, map1to2, map2to1);
}

/***
    match_corners:  Compare the lists of corners, remembering those that are
                    similar and close in position.
    args:           corners1 - corners to match, in corners2 coords
                    corners2 - corners to be matched
                    tree - sorted tree of corners2 points
                    map[12]to[21] - corners matching (c2 for c1 or v.v.)
    returns:   number of matched corners
    modifies:  map1to2, map2to1
***/
proc match_corners(const corners1 : [] corner, const corners2 : [] corner,
                   tree : kdtree, map1to2 : [] int, map2to1 : [] int) : int {
  var nmap : int;                       /* number of matching corners */

  map1to2 = -1;
  map2to1 = -1;

  /* We do not try to disambiguate multiple matches, nor do we attempt to
     look at other corner2 points if the nearest isn't similar.  That can
     get ugly.  If the scale is about 1 and mapsep < suppsep, there shouldn't
     be a problem.  But extreme scaling can break this.
     Note that this is a serial process - it's a winner-take-all strategy.
  */
  for c1 in corners1.domain {
    if (0 <= map1to2(c1)) {
      continue;
    }
    const c2 = tree.find_nearest_point(corners1(c1).center);
    const (xc, yc) = corners1(c1).center;
    const (vc, wc) = corners2(c2).center;
    if ((map2to1(c2) < 0) &&
        (abs(xc-vc) <= mapsep) &&
        (abs(yc-wc) <= mapsep) &&
        are_corners_similar(corners1(c1), corners2(c2))) {
      nmap += 1;
      map1to2(c1) = c2;
      map2to1(c2) = c1;
    }
  }

  return nmap;
}

/***
    select_besttry:  Pick the best mapping we've found (most matching corners).
                     Note nfail in the data structure will be equal to nfail
                     the command line constant if we have no solution - that
                     try will be ignored.
    args:            tries - information about the attempts resulting in maps
    returns:   index into tries with best result
***/
proc select_besttry(tries : [] tryinfo) : int {
  var bestind : int;                    /* index of best mapping */

  bestind = 0;
  /* Serial reduction. */
  for ind in tries.domain {
    if ((nfail != tries(ind).nfail) && (0 < tries(ind).nmap)) {
      if (0 == bestind) {
        bestind = ind;
      } else if (tries(bestind).nmap < tries(ind).nmap) {
        bestind = ind;
      }
    }
  }

  if (0 == bestind) {
    halt("no mapping found");
  }

  return bestind;
}

/***
    refine_mapping:  Determine a best-fit mapping of the matches saved in the
                     map[12]to[21] arrays with the try.  Basically a
                     regression.  Note that corners1 should be in its original
                     coordinates.
    args:            corners1, corners2 - list of corners matched
                     map[12]to[21] - corners matching (c2 for c1 or v.v.)
                     bestmap - final mapping
    modifies:  bestmap
***/
proc refine_mapping(const corners1 : [] corner, const corners2 : [] corner,
                    const map1to2 : [] int, const map2to1 : [] int,
                    out bestmap : mapinfo) {
  var seedmap : mapinfo;                /* mapping from seeds */

  /* Cannot use a cobegin here because bestmap is an output. */
  regress_coords(corners1, corners2, map1to2, 0,
                 bestmap.sxx, bestmap.sxy, bestmap.dx);
  regress_coords(corners1, corners2, map1to2, 1,
                 bestmap.syx, bestmap.syy, bestmap.dy);
  affine_to_rst(bestmap);

  /* For debug, not needed in prdouction. */
  dump_map("Refined map", bestmap);
  /* bestmap may be invalid, say because the rigidity ratios have tweaked
     slightly and gone OOB.  We allow that, but if you wanted to play safe
     you could regenerate the mapping from the seeds, which we know is good. */
}

/***
    regress_coords:  Perform a regression of the equation v = Ax + By + C,
                     where v is the xc or yc center coordinates of a corners2
                     point (as determined by the cbase argument).
    args:            corners1, corners2 - list of corners we've matched
                     map1to2 - corners2 index corresponding to corners1
                               < 0 if no match, ie. skip
                     nmap - number of corners mapped
                     cbase - coordinate in corners2 to regress, 1 xc 2 yc
                     A, B, C - mapping equation coefficients
    modifies:  A, B, C
***/
proc regress_coords(const corners1 : [] corner, const corners2 : [] corner,
                    map1to2 : [] int, cbase : int,
                    out A : real, out B : real, out C : real) {
  var sumx, sumy, sumx2, sumy2 : int;   /* partial sums for fit */
  var sumxy, sumv, sumxv, sumyv : int;
  var k, l, m, p, r : real;             /* intermediate terms */
  var nmap : int;                       /* number of mapped corners */

  /* Here 'x' and 'y' are the corners1 centers and 'v' the x or y center
     of corners2.  Do not do in parallel, the sums are not safe. */
  for c1 in map1to2.domain {
    if (0 <= map1to2(c1)) {
      const c2 = map1to2(c1);           /* matching corner */
      const (xc, yc) = corners1(c1).center;
      const v = corners2(c2).center(cbase);
      sumx += xc;
      sumx2 += xc * xc;
      sumy += yc;
      sumy2 += yc * yc;
      sumxy += xc * yc;
      sumv += v;
      sumxv += xc * v;
      sumyv += yc * v;
      nmap += 1;
    }
  }

  k = nmap * sumxv - sumx * sumv;
  l = sumx * sumx - nmap * sumx2;
  m = sumx * sumy - nmap * sumxy;
  p = nmap * sumyv - sumy * sumv;
  r = sumy * sumy - nmap * sumy2;

  A = (k * r - m * p) / (m * m - l * r);
  B = -(k + l * A) / m;
  C = (sumv - A * sumx - B * sumy) / nmap;
}


/***
    are_corners_similar:  Compare two corners to see if they match.  The
                          length of the differing segment must be about the
                          same as well as the start point and difference
                          direction.
    args:                 c1, c2 - corners to compare
    returns:   true if similar, false if not
***/
inline proc are_corners_similar(c1 : corner, c2 : corner) : bool {
  const (xc, yc) = c1.center;           /* corner point of c1 */
  const (xp, yp) = c1.st;               /* start point of segment in c1 */
  const (vc, wc) = c2.center;           /* corner point of c2 */
  const (vp, wp) = c2.st;               /* start point of segment in c2 */
  const dx = xp - xc;                   /* x offset of c1 start point */
  const dy = yp - yc;                   /* y offset of c1 start point */
  const dv = vp - vc;                   /* x offset of c2 start point */
  const dw = wp - wc;                   /* y offset of c2 start point */

  if (dlen < abs(c1.len - c2.len)) then return false;
  if (c1.dir != c2.dir) then return false;
  if (dst < abs(dx - dv)) then return false;
  if (dst < abs(dy - dw)) then return false;

  return true;
}


/***
    random_ranged:  Return a random number in the range, incl.  Assumes integer
                    type.
    args:           rand - random number stream
                    rng - range to get random number within
    returns:   random point within range
***/
proc random_ranged(rand : RandomStream?, rng : range) : rng.idxType {
  const elt = rng.size * rand!.getNext();      /* scale random to range */

  /* It would be nice to have a method on ranges that returns the i'th
     value.  That way we wouldn't have to worry about the type. */
  return rng.first + rng.stride * (nearbyint(elt - 0.5) : rng.idxType);
}

/***
    random_domain:  Returns an index of a random point in the domain.
    args:           rand - random number generator
                    dom - domain to generate point in
    returns:   random point within domain
***/
proc random_domain(rand : RandomStream?, dom : domain) : dom.rank * dom.idxType {
  /* Do not use dom.numIndices.  dom.rank is a param, and the tuple declaration
     requires a parameter. */
  var pt : dom.rank * dom.idxType;

  for param i in 0..(dom.rank-1) do pt(i) = random_ranged(rand, dom.dim(i));

  return pt;
}

/***
    random_bound:  Return a number in the range, incl.
    args:          rand - random number generator
                   rmin, rmax - bounds of range (incl.)
    returns:   random number from rmin t/m rmax
***/
proc random_bound(rand : RandomStream?, rmin : int, rmax : int) : int {
  const len = (rmax - rmin + 1) : real;
  const elt = len * rand!.getNext();             /* scale random to range */
  const res = rmin + nearbyint(elt-0.5) : int;  /* random number */
  return res;
}


/***
    dump_tries:  Print a summary of the runs made - number completed, number
                 of times re-seeding was necessary - as well as the best
                 result.
    args:        tries - information about attempts to match
***/
proc dump_tries(tries : [] tryinfo) {
  var totfail : int;                    /* total number of re-seedings */
  var ttot : real;                      /* total time spent on all tries */
  var nsucceed : int;                   /* number tries that have a result */
  var maxnmap : int;                    /* maximum number matching corners */

  for t in tries {
    if (nfail != t.nfail) then nsucceed += 1;
    if (maxnmap < t.nmap) then maxnmap = t.nmap;
    totfail += t.nfail;
    ttot += t.ttry;
  }

  writeln("\nSummary of fits");
  writef("  number maps attempted      %4i\n", tries.size);
  writef("  number with result         %4i     (%3.0dr%%)\n", nsucceed,
         (100.0 * nsucceed) / tries.size);
  writef("  best number matches        %4i\n", maxnmap);
  writef("  avg re-seeds per try       %6.1dr   (total %6i)\n",
         (1.0 * totfail) /  tries.size, totfail);
  writef("  avg re-seeds per success   %6.1dr\n",
         (1.0 * (totfail - nfail * (tries.size - nsucceed))) / nsucceed);
  writef("  avg time per try [ms]     %7.1dr\n", ttot / tries.size);
}

/***
    dump_map:  Print a mapping with a title to stdout.
    args:      title - name of the map (header line)
               map - transform from image1 to image2
***/
proc dump_map(title : string, map : mapinfo) {

  writeln("\n", title);
  writef("  x2 = %6.3dr x1  %+6.3dr y1  %+7.1dr\n",
         map.sxx, map.sxy, map.dx);
  writef("  y2 = %6.3dr x1  %+6.3dr y1  %+7.1dr\n",
         map.syx, map.syy, map.dy);
  writef("or\n");
  writef("  x2 =  %6.3dr cos(theta) x1  %+6.3dr sin(theta) y1  %+7.1dr\n",
         map.sx, map.sx, map.dx);
  writef("  y2 =  %6.3dr sin(theta) x1  %+6.3dr cos(theta) y1  %+7.1dr\n",
         map.sy, map.sy, map.dy);
  writef("  theta = %7.4dr rad  or  %6.1dr deg\n",
         map.theta, 180.0 * map.theta / pi);
}

/***
    draw_matches:  Create and save an image with both (greyscale) input images
                   and color lines between matching corners.  Routine will
                   bail (via cleanup_onerr) if there's a problem working with
                   the images.  Images will be written to the global const
                   outname - if it's empty, routine does nothing.
    args:          img1, img2 - input images
                   corners1, corners2 - corners found in inputs
                   match1to2 - index of corners2 matching 1, < 0 if no match
***/
proc draw_matches(img1 : unmanaged clrimage?, img2 : unmanaged clrimage?,
                  corners1 : [] corner, corners2 : [] corner,
                  match1to2 : [] int) : int {
  var disp : c_ptr(rgbimage);                /* output image */
  var grey : c_ptr(rgbimage);                /* greyscale version of input */
  var img2grey : conversion;                 /* greyscale conversion spec */
  var colors : [1..2*match_nclr] 3*c_uchar;  /* colors to use for lines */
  var ncol, nrow : c_int;                    /* size of combined image */
  var x1off, y1off : int;                    /* offset of image1 in combined */
  var x2off, y2off : int;                    /* offset of image2 in combined */
  var cnt : int;                             /* running count lines drawn */
  var retval : int;

  /* Not an error, just leave quietly. */
  if ("" == outname) then return -1;

  setup_combiimg(img1, img2, ncol, nrow, x1off, y1off, x2off, y2off);

  retval = alloc_rgbimage(disp, ncol, nrow);
  end_onerr(retval, disp);

  /* Copy over images */
  img2grey.plane = clrplane.C1;
  if (clrspace.YUV == space) {
    img2grey.how = rgbconvert.CLIP;
  } else {
    img2grey.how = rgbconvert.BOUND;
    img2grey.min = LAB_LMIN;
    img2grey.max = LAB_LMAX;
  }

  retval = display_color(img1, grey, img2grey);
  end_onerr(retval, disp, grey);
  for y in img1!.rows {
    for x in img1!.cols {
      const xy = y * img1!.ncol + x;
      const xyd = (y + y1off) * ncol + (x + x1off);
      disp.deref().r(xyd) = grey.deref().r(xy);
      disp.deref().g(xyd) = grey.deref().g(xy);
      disp.deref().b(xyd) = grey.deref().b(xy);
    }
  }

  retval = display_color(img2, grey, img2grey);
  end_onerr(retval, disp, grey);
  for y in img2!.rows {
    for x in img2!.cols {
      const xy = y * img2!.ncol + x;
      const xyd = (y + y2off) * ncol + (x + x2off);
      disp.deref().r(xyd) = grey.deref().r(xy);
      disp.deref().g(xyd) = grey.deref().g(xy);
      disp.deref().b(xyd) = grey.deref().b(xy);
    }
  }

  /* Prepare colors.  Twice around the HSV circle, the second time at half
     value. */
  for c in 1..match_nclr {
    var r, g, b : c_uchar;              /* back-converted color */
    hsvpix_to_rgb((c-1) * 360.0/match_nclr, 1.0, 1.0, r, g, b);
    colors(c) = (r, g, b);
  }
  for c in (match_nclr+1)..(2*match_nclr) {
    var r, g, b : c_uchar;              /* back-converted color */
    hsvpix_to_rgb((c-1-match_nclr) * 360.0/match_nclr, 1.0, 0.5, r, g, b);
    colors(c) = (r, g, b);
  }

  cnt = 0;
  for c1 in match1to2.domain {
    const c2 = match1to2(c1);
    if (1 <= c2) {
      const (xc1, yc1) = corners1(c1).center;
      const (xc2, yc2) = corners2(c2).center;
      /*
      writef("draw line from   %4i,%4i   to   %4i,%4i\n",
             xc1+x1off,yc1+y1off, xc2+x2off,yc2+y2off);
      */
      draw_line(disp, colors(cnt%(2*match_nclr)), (xc1+x1off, yc1+y1off),
                (xc2+x2off, yc2+y2off));
      cnt += 1;
    }
  }

  retval = PNG_write(outname.c_str(), disp, CLR_RGB);
  end_onerr(retval, disp, grey);

  free_rgbimage(disp);
  free_rgbimage(grey);

  return 0;
}

/***
    setup_combiimg:  Determine the layout of an image that holds two others
                     with a margin between them.
    args:            img1, img2 - images to combine
                     ncol, nrow - size of combined image
                     xoff1, yoff1 - position of (0,0) of img1 in combined
                     xoff2, yoff2 - position of (0,0) of img2 in combined
    modifies:  ncol, nrow, xoff1, yoff1, xoff2, yoff2
***/
proc setup_combiimg(img1 : unmanaged clrimage?, img2: unmanaged clrimage?,
                    out ncol : c_int, out nrow : c_int,
                    out x1off : int, out y1off : int,
                    out x2off : int, out y2off : int) {

  ncol = (img1!.ncol + match_margin + img2!.ncol) : c_int;
  x1off = 0;
  x2off = img1!.ncol + match_margin;

  if (img1!.nrow == img2!.nrow) {
    nrow = img1!.nrow : c_int;
    y1off = 0;
    y2off = 0;
  } else if (img1!.nrow < img2!.nrow) {
    nrow = img2!.nrow : c_int;
    y1off = (img2!.nrow - img1!.nrow) / 2;
    y2off = 0;
  } else {
    nrow = img1!.nrow : c_int;
    y1off = 0;
    y2off = (img1!.nrow - img2!.nrow) / 2;
  }
}

/***
    draw_line:  Draw a colored line in an image.  Anything underneath will be
                overwritten.  Implements Bresenham's line algorithm (see
                Wikipedia).
    args:       img - where to draw
                clr - (r, g, b) triple
                stpt - first point of line
                endpt - last point of line
    modifies:  img
***/
proc draw_line(img : c_ptr(rgbimage), clr : 3 * c_uchar, stpt : 2 * int,
               endpt : 2 * int) {
  const (x0, y0) = stpt;
  const (x1, y1) = endpt;
  const dx = x0 - x1;                   /* slope components */
  const dy = y0 - y1;
  var octant                            /* which octant we're in */
    = determine_octant(x0, y0, x1, y1);

  select (octant) {
  when 1 do draw_line_octant1(img, clr, x0, y0, x1, y1);
  when 2 do draw_line_octant2(img, clr, x0, y0, x1, y1);
  when 3 do draw_line_octant3(img, clr, x0, y0, x1, y1);
  when 4 do draw_line_octant8(img, clr, x1, y1, x0, y0);
  when 5 do draw_line_octant1(img, clr, x1, y1, x0, y0);
  when 6 do draw_line_octant2(img, clr, x1, y1, x0, y0);
  when 7 do draw_line_octant3(img, clr, x1, y1, x0, y0);
  when 8 do draw_line_octant8(img, clr, x0, y0, x1, y1);
  }
}

/***
    determine_octant:  Determine which octant the line is drawn in.
    args:              x0, y0 - start point of line
                       x1, y1 - end point of line
    returns:   octant 1-8
***/
proc determine_octant(x0 : int, y0 : int, x1 : int, y1 : int) : int {
  const dx = x1 - x0;                   /* slope components */
  const dy = y1 - y0;

  if ((0 <= dx) && (0 <= dy)) {
    if (dy <= dx) {
      return 1;
    } else {
      return 2;
    }
  } else if ((dx <= 0) && (0 <= dy)) {
    if (-dx < dy) {
      return 3;
    } else {
      return 4;
    }
  } else if ((dx <= 0) && (dy <= 0)) {
    if (-dy <= -dx) {
      return 5;
    } else {
      return 6;
    }
  } else {
    if (dx < -dy) {
      return 7;
    } else {
      return 8;
    }
  }
}

/***
    draw_line_octant1:  Draw a colored line in an image, overwriting anything
                        underneath.  Implements Bresenham's line algorithm
                        (see Wikipedia) in the first octant, ie. 0 < dy < dx.
    args:               img - where to draw
                        clr - (r, g, b) triple of color of line
                        x0, y0 - first point of line
                        x1, y1 - first point of line
    modifies:  img
***/
proc draw_line_octant1(img : c_ptr(rgbimage), clr : 3 * c_uchar, x0 : int, y0 : int,
                       x1 : int, y1 : int) {
  const (r, g, b) = clr;                /* destructuring */
  const dx = x1 - x0;                   /* slope components */
  const dy = y1 - y0;
  var x, y, xy : int;                   /* pixel coordinates, index to image */
  var err : int;                        /* cum error in point placement */

  y = y0;
  x = x0;
  err = 0;
  while (x <= x1) {
    xy = (y * img.deref().ncol) + x;
    img.deref().r(xy) = r;
    img.deref().g(xy) = g;
    img.deref().b(xy) = b;

    err += dy;
    if (dx <= (2 * err)) {
      y += 1;
      err -= dx;
    }
    x += 1;
  }
}

/***
    draw_line_octant2:  Draw a colored line in an image, overwriting anything
                        underneath.  Only valid in the second octant, ie,
                        0 < dx < dy.
    args:               img - where to draw
                        clr - (r, g, b) triple of color of line
                        x0, y0 - first point of line
                        x1, y1 - first point of line
    modifies:  img
***/
proc draw_line_octant2(img : c_ptr(rgbimage), clr : 3 * c_uchar, x0 : int, y0 : int,
                       x1 : int, y1 : int) {
  const (r, g, b) = clr;                /* destructuring */
  const dx = x1 - x0;                   /* slope components */
  const dy = y1 - y0;
  var x, y, xy : int;                   /* pixel coordinates, index to image */
  var err : int;                        /* cum error in point placement */

  y = y0;
  x = x0;
  err = 0;
  while (y <= y1) {
    xy = (y * img.deref().ncol) + x;
    img.deref().r(xy) = r;
    img.deref().g(xy) = g;
    img.deref().b(xy) = b;

    err += dx;
    if (dy <= (2 * err)) {
      x += 1;
      err -= dy;
    }
    y += 1;
  }
}

/***
    draw_line_octant3:  Draw a colored line in an image, overwriting anything
                        underneath.  Only valid in the third octant, ie,
                        dx < 0, 0 < dy, and -dx < dy.
    args:               img - where to draw
                        clr - (r, g, b) triple of color of line
                        x0, y0 - first point of line
                        x1, y1 - first point of line
    modifies:  img
***/
proc draw_line_octant3(img : c_ptr(rgbimage), clr : 3 * c_uchar, x0 : int, y0 : int,
                       x1 : int, y1 : int) {
  const (r, g, b) = clr;                /* destructuring */
  const dx = x1 - x0;                   /* slope components */
  const dy = y1 - y0;
  var x, y, xy : int;                   /* pixel coordinates, index to image */
  var err : int;                        /* cum error in point placement */

  y = y0;
  x = x0;
  err = 0;
  while (y1 <= y) {
    xy = (y + img.deref().ncol) * x;
    img.deref().r(xy) = r;
    img.deref().g(xy) = g;
    img.deref().b(xy) = b;

    err += dx;
    if ((2 * err) <= -dy) {
      x -= 1;
      err += dy;
    }
    y += 1;
  }
}

/***
    draw_line_octant8:  Draw a colored line in an image, overwriting anything
                        underneath.  Only valid in the eigth octant, ie,
                        0 < dx, dy < 0, and -dy < dx.
    args:               img - where to draw
                        clr - (r, g, b) triple of color of line
                        x0, y0 - first point of line
                        x1, y1 - first point of line
    modifies:  img
***/
proc draw_line_octant8(img : c_ptr(rgbimage), clr : 3 * c_uchar, x0 : int, y0 : int,
                       x1 : int, y1 : int) {
  const (r, g, b) = clr;                /* destructuring */
  const dx = x1 - x0;                   /* slope components */
  const dy = y1 - y0;
  var x, y, xy : int;                   /* pixel coordinates, index to image */
  var err : int;                        /* cum error in point placement */

  y = y0;
  x = x0;
  err = 0;
  while (x1 <= x) {
    xy = (y + img.deref().ncol) * x;
    img.deref().r(xy) = r;
    img.deref().g(xy) = g;
    img.deref().b(xy) = b;

    err += dy;
    if ((2 * err) <= -dx) {
      y -= 1;
      err += dx;
    }
    x += 1;
  }
}


/**** Debug ****/

/**
   The main loop in align_corners was being run on only one or two CPU cores.
   To debug, Greg Titus at Cray had used a coforall over maxTaskPar to force
   one thread per core, and used a variable to pick off tries.  That way if
   one thread got bogged down the others would pick up the slack.  We tracked
   the timing of each seed attempt to see how they were executed across the
   threads.  For historical purposes we include this code here - we don't
   want to use it in production.  It turns out that moving the random number
   stream into the (co)forall causesd the problem to go away - this is what's
   used now.  (attemptinfo collects timing info because Greg saw that the
   time per try varied wildly, because one or two threads were basically
   stopped by the RNG; this debug code was to verify the effect and, together
   with timing code in pick_seeds, to narrow down where the delays occurred.)
**/

record attemptinfo {
  var tattempt : real;                  /* time in ms for attempt */
  var tseed : real;                     /* time in ms for seeding */
  var fail : bool;                      /* whether passed or not */
  var seeded : bool;                    /* whether seeds found or not */
  var thread : int;                     /* which thread/CPU it ran on */
  var seed1 : 2*int;                    /* seeds used */
  var seed2 : 2*int;
  var seed3 : 2*int;
}

/***
    align_corners:  Try random pairs of corners to determine the mapping from
                    one image to another.
    args:           corners1 - corners found in first image
                    corners2 - corners found in second image
                    bestmap - best alignment we've found
                    map1to2, map2to1 - corners that match up, by index
    modifies:  bestmap, map1to2, map2to1
***/
proc align_corners_dbg(const corners1 : [] corner, const corners2 : [] corner,
                       out bestmap : mapinfo,
                       map1to2 : [] int, map2to1 : [] int) {
  var tries : [1..ntry] tryinfo;        /* details about each try */
  var besttry : int;                    /* try with best result */
  var rand = createRandomStream(real);  /* random numbers to pick seeds */
  const minlen                          /* smallest number of corners */
    = min(corners1.domain.dim(0).last, corners2.domain.dim(0).last);
  const mincnt                          /* number corners that must match */
    = nearbyint(matchfrac * minlen) : int;
  var tree                              /* 2-dimensional sorted space */
    = new unmanaged kdtree(corners2.domain.size, int, 2);

  for i in corners2.domain {
    tree.add_node(corners2(i).center, i);
  }
  tree.assemble_tree();

  var attempts : [1..ntry][0..nfail] attemptinfo;
  var nexttry : atomic int;

  for trial in 1..ntry {
    for fail in 1..nfail {
      attempts(trial)(fail).tattempt = 0.0;
      attempts(trial)(fail).fail = true;
      attempts(trial)(fail).thread = -1;
    }
  }
  nexttry.write(1);

  coforall thr in 1..here.maxTaskPar with (ref rand) {
    while (true) {
      const trial = nexttry.fetchAdd(1);
      var rand_local = createRandomStream(parSafe=false);

      if (ntry < trial) then break;

      var trytime : stopwatch;
      trytime.start();
      do {
        attempts(trial)(tries(trial).nfail).thread = thr;

        var failtime : stopwatch;
        var seedtime : stopwatch;
        failtime.start();
        seedtime.start();
        const picked = pick_seeds(corners1, corners2, rand_local, tries(trial));
        seedtime.stop();
        attempts(trial)(tries(trial).nfail).seeded = picked;
        attempts(trial)(tries(trial).nfail).tseed =
          seedtime.elapsed() * 1_000;

        if (picked) {
          attempts(trial)(tries(trial).nfail).seed1 = tries(trial).seed1;
          attempts(trial)(tries(trial).nfail).seed2 = tries(trial).seed2;
          attempts(trial)(tries(trial).nfail).seed3 = tries(trial).seed3;

          var mapped1 : [corners1.domain] corner;
          map_corners(corners1, tries(trial).map, mapped1);
          const matchcnt = count_matches(mapped1, corners2, tree);

          failtime.stop();
          attempts(trial)(tries(trial).nfail).tattempt =
            failtime.elapsed() * 1_000;

          if (mincnt <= matchcnt) {
            tries(trial).nmap = matchcnt;
            attempts(trial)(tries(trial).nfail).fail = false;
            break;
          }
        } else {
          failtime.stop();
          attempts(trial)(tries(trial).nfail).tattempt =
            failtime.elapsed() * 1_000;
        }
        tries(trial).nfail += 1;
      } while (tries(trial).nfail < nfail);

      trytime.stop();
      tries(trial).ttry = trytime.elapsed() * 1_000;

      delete rand_local;
    }
  }

  analyze_attempts(tries, attempts);

  besttry = select_besttry(tries);

  /**!!!**/
  /* This is for debug, don't need in production. */
  dump_map("Seed map", tries(besttry).map);

  /* We've avoided storing arrays in the records as it seems to be a problem
     with passing as arguments.  We need to regenerate the mapping for the
     best try because of it (before had stored map[12]to[21] with tryinfo). */
  var mapped1 : [corners1.domain] corner;
  map_corners(corners1, tries(besttry).map, mapped1);
  match_corners(mapped1, corners2, tree, map1to2, map2to1);
  refine_mapping(corners1, corners2, map1to2, map2to1, bestmap);

  dump_tries(tries);

  delete rand;
  delete tree;
}

/***
    analyze_attempts:  Print a table summarizing the timing information per
                       try and attempt and outliers.
    args:              tries - info collected per try
                       attempts - info collected per attempt
***/
proc analyze_attempts(tries : [1..ntry] tryinfo,
                      attempts : [1..ntry][0..nfail] attemptinfo) {
  const nthr = here.maxTaskPar;         /* number of threads (abbrev) */
  var tthread : [1..nthr] real;         /* total time per thread */
  var thrcnt : [1..nthr] int;           /* number attempts per thread */
  var thrhist : [1..nthr] int;          /* count tries / thread */
  var thist : [-2..10] int;             /* order magnitude histogram ttry */
  var ahist : [-2..10] int;             /* order magnitude tattempt */
  var shist : [-2..10] int;             /* order magnitude tseed */
  var sumt, sum2t : real;               /* stat for ttry */
  var suma, sum2a : real;               /* stat for tattempt */
  var sums, sum2s : real;               /* stat for tseed */
  var sump, sum2p : real;               /* stat for processing seeds time */
  var totattempt : int;                 /* total number of attempts */
  var nproc : int;                      /* number of seeds to matches */

  writeln("\n---- raw attempt info");
  for trial in 1..ntry {
    const l10_t = floor(log10(tries(trial).ttry)) : int;
    sumt += tries(trial).ttry;
    sum2t += tries(trial).ttry ** 2;
    const tind =
      if (l10_t < -2) then -2 else if (10 < l10_t) then 10 else l10_t;
    thist(tind) += 1;
    /* Note this is at the trial level.  We're sure to have attempt 0 to get
       the thread. */
    var trythr = attempts(trial)(0).thread;
    thrhist(trythr) += 1;

    writef("trial %3i  nfail %4i  nmap %4i  thr %2i  t %8.1dr (%2i)  seed (%4i,%4i) (%4i,%4i) (%4i,%4i)\n",
           trial, tries(trial).nfail,tries(trial).nmap, trythr,
           tries(trial).ttry, l10_t,
           tries(trial).seed1(1),tries(trial).seed1(2),
           tries(trial).seed2(1),tries(trial).seed2(2),
           tries(trial).seed3(1),tries(trial).seed3(2));

    const nattempt =
      if (tries(trial).nfail == nfail) then nfail-1 else tries(trial).nfail;
    for a in 0..nattempt {
      const l10_s = floor(log10(attempts(trial)(a).tseed)) : int;
      const l10_a = floor(log10(attempts(trial)(a).tattempt)) : int;
      const aind =
        if (l10_a < -2) then -2 else if (10 < l10_a) then 10 else l10_a;
      ahist(aind) += 1;
      const sind =
        if (l10_s < -2) then -2 else if (10 < l10_s) then 10 else l10_s;
      shist(sind) += 1;
      tthread(attempts(trial)(a).thread) += attempts(trial)(a).tattempt;
      thrcnt(attempts(trial)(a).thread) += 1;
      totattempt += 1;
      suma += attempts(trial)(a).tattempt;
      sum2a += attempts(trial)(a).tattempt ** 2;
      sums += attempts(trial)(a).tseed;
      sum2s += attempts(trial)(a).tseed ** 2;

      if (attempts(trial)(a).seeded) {
        const tproc = attempts(trial)(a).tattempt - attempts(trial)(a).tseed;
        nproc += 1;
        sump += tproc;
        sum2p += tproc ** 2;

        writef("attempt %4i  thr %1i  t %8.1dr (%2i)  tseed %8.1dr (%2i)  tproc %8.1dr  seed (%4i,%4i) (%4i,%4i) (%4i,%4i)\n",
               a, attempts(trial)(a).thread, attempts(trial)(a).tattempt, l10_a,
               attempts(trial)(a).tseed, l10_s, tproc,
               attempts(trial)(a).seed1(1),attempts(trial)(a).seed1(2),
               attempts(trial)(a).seed2(1),attempts(trial)(a).seed2(2),
               attempts(trial)(a).seed3(1),attempts(trial)(a).seed3(2));
      } else {
        writef("attempt %4i  thr %1i  t %8.1dr (%2i)  tseed %8.1dr (%2i)  tproc %8.1dr\n",
               a, attempts(trial)(a).thread, attempts(trial)(a).tattempt, l10_a,
               attempts(trial)(a).tseed, l10_s,
               attempts(trial)(a).tattempt - attempts(trial)(a).tseed);
      }
    }
  }
  writeln("--- end attempt info\n");

  writef("\n                   trial   attempt       seed");
  if (0 < nproc) {
    writef("       proc");
  }
  writef("\navg            %8.1dr   %8.1dr   %8.1dr",
         sumt / ntry, suma / totattempt, sums / totattempt);
  if (0 < nproc) {
    writef("   %8.1dr   (cnt %6i)", sump / nproc, nproc);
  }
  writef("\nstdev          %8.1dr   %8.1dr   %8.1dr",
         sqrt((sum2t - (sumt ** 2 / ntry)) / (ntry - 1)),
         sqrt((sum2a - (suma ** 2 / totattempt)) / (totattempt - 1)),
         sqrt((sum2s - (sums ** 2 / totattempt)) / (totattempt - 1)));
  if (0 < nproc) {
    writef("   %8.1dr\n",
           sqrt((sum2p - (sump ** 2 / nproc)) / (nproc - 1)));
  } else {
    writef("\n");
  }
  for i in -2..10 {
    writef("log hist  %2i   %8i   %8i   %8i\n", i, thist(i),ahist(i),shist(i));
  }

  writeln("\n\nthread   tries per   t total      t avg");
  for i in 1..nthr {
    if (0 == thrcnt(i)) {
      writef("%2i          %3i\n", i, thrhist(i));
    } else {
      writef("%2i          %3i     %8.1dr   %8.1dr\n",
             i, thrhist(i), tthread(i), tthread(i) / thrcnt(i));
    }
  }

  writeln("\n");
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
  writeln("  --radius must be >= 3");
  writeln("  note L, Y scales differ, default --thr set for L");
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

  if ("" == inname1) then usage("missing --inname1");
  if (!PNG_isa(inname1.c_str())) then usage("input file 1 not a PNG picture");
  if ("" == inname2) then usage("missing --inname2");
  if (!PNG_isa(inname2.c_str())) then usage("input file 2 not a PNG picture");
  if ((clrspace.LAB != space) && (clrspace.YUV != space)) then
    usage("only use LAB or YUV color spaces");
  if (radius < 3) then usage("radius too small");
}


proc main() {
  var rgb : c_ptr(rgbimage);            /* image we've read */
  var clr1 : unmanaged clrimage?;       /* greyscale first image */
  var clr2 : unmanaged clrimage?;       /* greyscale second image */
  var spec : fastspec;                  /* FAST parameters */
  var corners1 : unmanaged chunkarray(corner)?;    /* corners found in clr1 */
  var corners2 : unmanaged chunkarray(corner)?;    /* corners found in clr2 */
  var Lcnr1 : domain(rank=1);           /* domain of corners1 */
  var Lcnr2 : domain(rank=1);           /* domain of corners2 */
  var match1to2 : [Lcnr1] int;          /* corners2 index for each corners1 */
  var match2to1 : [Lcnr2] int;          /* corners1 index for each corners2 */
  var bestmap : mapinfo;                /* best mapping between images */
  var retval : int;

  verify_setup();

  retval = PNG_read(inname1.c_str(), rgb);
  end_onerr(retval, rgb);
  rgb_convert(rgb, clr1, space);

  retval = PNG_read(inname2.c_str(), rgb);
  end_onerr(retval, rgb, clr1);
  rgb_convert(rgb, clr2, space);

  spec.radius = radius;
  spec.minlen = minlen;
  spec.maxlen = maxlen;
  spec.thr = thr;
  spec.suppsep = suppsep;

  corners1 = find_corners(clr1, spec);
  corners2 = find_corners(clr2, spec);

  Lcnr1 = corners1!().domain;
  Lcnr2 = corners2!().domain;

  align_corners(corners1!(), corners2!(), bestmap, match1to2, match2to1);

  dump_map("Best alignment", bestmap);

  draw_matches(clr1, clr2, corners1!(), corners2!(), match1to2);

  free_rgbimage(rgb);
  delete clr1;
  delete clr2;
  delete corners1;
  delete corners2;

  return 0;
}
