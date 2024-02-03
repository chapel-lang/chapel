
/*****
      ransac_st.chpl -
      Use the RANSAC method to align to images.  Accounts for scaling and
      translation (st), not rotation, ie.
           x' = sx * x + dx
           y' = sy * y + dy
      Uses corners from the FAST detector as features.  Prints the best fit
      information when done.

      Call:
        ransac
          --inname1=<file>         first image to compare
          --inname2=<file>         second image to compare
          --space=[LAB|YUV]        color space for greyscale conversion
          --radius=#               radius of FAST detector ring
          --minlen=#               minimum length consecutive series at corner
          --maxlen=#               maximum length consecutive series at corner
          --thr=#                  pixel difference from center of ring
          --suppsep=#              x, y difference to suppress one corner
          --ntry=#                 number random seeds to try
          --nfail=#                number times to re-seed if match fails
          --mapsep=#               corners after mapping must be this close
          --matchfrac=#            fraction of corners that must match to pass
          --dlen=#                 length diff b/t corners to be same
          --dst=#                  x, y delta in start offsets to be same
          --fixrng=[true|false]    compile param to fix seed for RNG

      c 2015-2018 Primordial Machine Vision Systems
*****/

/**
   tests:
     ammo1 vs. ammo2    sx 1.25 dx 150   sy 1.30 dy 300
     bond1 vs. bond2    sx 1.25 dx 150   sy 1.30 dy 300

   without/with kd-tree (times per try, not including building kd tree):
     ransac --inname1=bond1.png --inname2=bond2.png --thr=10 --ntry=10
            time/try [ms]     avg re-seed     % successful
     w/       8289                823              40
     w/o     41159                895              20

   - differences in number re-seeds, success rate seem high, but then w/o
     kd-tree picks first corner within mapsep and kd-tree picks closest
   - did not screen mapping after seeding for consistency
**/

use ip_color_v3;
use ip_corner;
use kdtree;
use Help;
use Random;
use Sort;
use Time;
use CTypes;
use Math;


/**** Command Line Arguments ****/

/* Input Files */
config const inname1 : string;          /* first image to compare */
config const inname2 : string;          /* second image to compare */
/* FAST Detector */
config const space = clrspace.LAB;      /* color space to work in */
config const radius = 3;                /* radius circle for FAST analysis */
config const minlen = 10;               /* shortest FAST run at corner */
config const maxlen = 13;               /* longest consecutive run at corner */
config const thr = 20.0;                /* threshold pixel differing center */
config const suppsep = 5;               /* x, y diff to suppress close corner */
/* RANSAC Alignment */
config const ntry = 50;                 /* number random maps to try */
config const mapsep = 5;                /* inlier distance after map */
config const matchfrac = 0.05;          /* min fraction corners must match */
config const dlen = 1;                  /* max delta length differing seq */
config const dst = 1;                   /* max dx dy of start pt around ring */
config const nfail = 1500;              /* how often to seed each try */
config param fixrng = false;            /* true to use known seed for RNG */
/* Mapping */
param SAME_S_RATIO = 1.25;              /* allowed ratio sx/sy or v.v. */
param MAXSCALE =     3.0;               /* max of sx, 1/sx, sy, or 1/sy */


/**** Data Structures ****/

record mapinfo {
  var sx, dx : real;                    /* x coordinate transform */
  var sy, dy : real;                    /* y coordinate transform */
}

record tryinfo {
  var seed1 : 2*int;                    /* (corner1,corner2) for one seed */
  var seed2 : 2*int;                    /* (corner1,corner2) for other seed */
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
                   ref map1to2 : [] int, ref map2to1 : [] int) {
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

  /* Using a coforall here kills performance, run times up by as much as an
     order of magnitude.  Putting rand outside the loop causes the program
     to only use one or two cores (why is still being checked).
  */
  forall i in 1..ntry {
    var rand : randomStream(real);      /* random numbers to pick seeds */
    var trytime : stopwatch;                /* run time for this try */

    if (fixrng) {
      rand = new randomStream(real, (2*i) + 1);
    } else {
      rand = new randomStream(real);
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
    pick_seeds:  Find two similar corners in each image.  Picks two arbitrary
                 pairs and determines if they form a valid mapping.  Seeds are
                 stored as (c1, c2) pairs of indices into the corners array
                 and may have garbage if the proc returns false.  Also
                 populates the mapping in the try based on the seeds (which
                 may also be garbage if it isn't valid).
    args:        corners1, corners2 - list of corners in each image
                 rand - random number generator
                 try - information about this attempt to match
    returns:   true if both seeds placed, false if cannot find two
    modifies:  try.seed[12], try.map
***/
proc pick_seeds(const corners1 : [] corner, const corners2 : [] corner,
                ref rand : randomStream(?), inout trial : tryinfo) : bool {
  var ind11, ind12 : int;               /* seed1 indices in corners1, 2 */
  var ind21, ind22 : int;               /* seed2 indices in corners1, 2 */
  var matches : [corners2.domain] int;  /* possible seeds in corners2 */
  var nmatch : int;                     /* number possible seeds */
  const rng1 = corners1.domain.dim(0);  /* range of indices for corners1 */
  const rng2 = corners2.domain.dim(0);  /* range of indices for corners2 */
  var retval = true;

  ind11 = random_ranged(rand, rng1);

  nmatch = 0;
  for i in rng2 {
    if are_corners_similar(corners1(ind11), corners2(i)) {
      nmatch += 1;
      matches(nmatch) = i;
    }
  }

  /**!!!**/
  /* Work around a Chapel bug here that double-frees the matches array if
     we were to return directly.  Also for the seed2 test.

     if (0 == nmatch) then return false;
  */
  if (0 == nmatch) {
    retval = false;
  } else {
    ind12 = matches(random_bound(rand, 1, nmatch));
    trial.seed1 = (ind11, ind12);

    do {
      ind21 = random_ranged(rand, rng1);
    } while (ind11 == ind21);

    /* No rotation, so we can require that they fall in the same quadrant.
       ie. If seed2 is in the second quadrant of the corners1 plane, then
       it must also be in the second quadrant of the corners2 plane. */
    const quad                          /* quadrant seed2 relative seed1 */
      = quadrant(corners1(ind11), corners1(ind21));
    nmatch = 0;
    for i in rng2 {
      if ((i != ind12) && are_corners_similar(corners1(ind21), corners2(i)) &&
          (quadrant(corners2(ind12), corners2(i)) == quad)) {
        nmatch += 1;
        matches(nmatch) = i;
      }
    }

    if (0 == nmatch) {
      retval = false;
    } else {
      ind22 = matches(random_bound(rand, 1, nmatch));
      trial.seed2 = (ind21, ind22);

      retval = map_seeds(corners1, corners2, trial.seed1, trial.seed2, trial.map);
    }
  }

  return retval;
}

/***
    map_seeds:  Give two seed pairs, determine the mapping from image1 to
                image2 coordinates.
    args:       corners1, corners2 - list of corners in each image
                seed1 - fisrt pair in each corners list to align
                seed2 - second pair in each corners list to align
                mapping - shift/scale to get seeds to align
    returns:   true if mapping acceptable, false if not
    modifies:  mapping.[sd]x, mapping.[sd]y
***/
proc map_seeds(corners1 : [] corner, corners2 : [] corner, seed1 : 2*int,
               seed2 : 2*int, inout mapping : mapinfo) : bool {
  /* First [12] is the seed, second [12] is the corners array. */
  const (ind11, ind12) = seed1;         /* seed 1 indices */
  const (ind21, ind22) = seed2;         /* seed 2 indices */
  const (xc1, yc1) = corners1(ind11).center;
  const (vc1, wc1) = corners2(ind12).center;
  const (xc2, yc2) = corners1(ind21).center;
  const (vc2, wc2) = corners2(ind22).center;

  mapping.sx = (vc2 - vc1) : real / (xc2 - xc1);
  mapping.dx = vc1 - (mapping.sx * xc1);
  mapping.sy = (wc2 - wc1) : real / (yc2 - yc1);
  mapping.dy = wc1 - (mapping.sy * yc1);

  return valid_map(mapping);
}

/***
    valid_map:  Do some sanity checking on the mapping.
    args:       mapping - shift/scale map between image coordinates
    returns:   true if mapping allowed, false if not
***/
proc valid_map(mapping : mapinfo) : bool {

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
  if ((mapping.sx <= 0.0) || (mapping.sy <= 0)) {
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
    modifies:  mapped
***/
proc map_corners(const corners : [] corner, const in mapping : mapinfo,
                 ref mapped : [] corner) {

  /* This gets everything; we'll overwrite the center and start. */
  mapped = corners;

  forall c in corners.domain with (const mapping, ref mapped) {
    const (xc, yc) = corners(c).center;
    const vc = nearbyint(mapping.sx * xc + mapping.dx) : int;
    const wc = nearbyint(mapping.sy * yc + mapping.dy) : int;

    mapped(c).center = (vc, wc);
    /* We shift the FAST ring start point along with the center, but do not
       scale it, as noted above. */
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
                   tree : kdtree(?)) : int {
  var map1to2 : [corners1.domain] int;  /* corners2 matching corners1 */
  var map2to1 : [corners2.domain] int;  /* corners1 matching corners2 */

  return match_corners(corners1, corners2, tree, map1to2, map2to1);
}

/***
    match_corners:  Compare the lists of corners, finding those that are
                    similar and close in position.
    args:           corners1 - corners to match, in corners2 coords
                    corners2 - corners to be matched
                    tree - sorted tree of corners2 points
                    map[12]to[21] - corners matching (c2 for c1 or v.v.)
    returns:   number of corners that match
    modifies:  map1to2, map2to1
***/
proc match_corners(const corners1 : [] corner, const corners2 : [] corner,
                   tree : kdtree(?), ref map1to2 : [] int, ref map2to1 : [] int) : int {
  var nmap : int;                       /* number of matching corners */

  /* Originally we used 'ref mapping : mapinfo' as an argument where the
     map[12]to[21] arrays were kept with mapinfo.  This produced NULL
     dereference crashes, not on every run, but often enough.  Moving the
     arrays into tries and passing it and an index, seems to solve the
     problem though.  But still there were instabilities, so we've moved
     to working directly on the arrays. */

  map1to2 = -1;
  map2to1 = -1;

  /* We do not try to disambiguate multiple matches.  That can get ugly.
     If the scale is about 1 and mapsep < suppsep, there shouldn't be a
     problem.  But extreme scaling can break this. */
  /* Do not do this in parallel - it's a winner-take-all strategy. */
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
    refine_mapping:  Determine a new mapping using the matches saved in the
                     map[12]to[21] arrays with the try.  Basically a linear
                     regression of all x's against each other, and all y's.
                     Note that corners1 should be in its original coordinates.
    args:            corners1, corners2 - list of corners matched
                     map[12]to[21] - corners matching (c2 for c1 or v.v.)
                     bestmap - final mapping
    modifies:  bestmap
***/
proc refine_mapping(const corners1 : [] corner, const corners2 : [] corner,
                    const map1to2 : [] int, const map2to1 : [] int,
                    out bestmap : mapinfo) {

  regress_coords(corners1, corners2, map1to2, 0, bestmap.sx, bestmap.dx);
  regress_coords(corners1, corners2, map1to2, 1, bestmap.sy, bestmap.dy);
}

/***
    regress_coords:  Perform a linear regression one coordinate of a corner
                     using the mapping in the try between the two sets.
    args:            corners1, corners2 - list of corners we're matching
                     map1to2 - corners2 index corresponding to corners1
                               < 0 if no match
                     cbase - coordinate to regress, 1 for xc, 2 for yc
                     m, b - slope and intercept, xc2 = m * xc1 + b
    modifies:  m, b
***/
proc regress_coords(corners1 : [] corner, corners2 : [] corner,
                    map1to2 : [] int, cbase : int,
                    out m : real, out b : real) {
  var sumx, sumv, sumxv, sumx2 : int;   /* partial sums for regression */
  var nmap : int;                       /* number of mapped corners */

  /* Do not do in parallel, the sums are not safe. */
  for c1 in map1to2.domain {
    if (0 <= map1to2(c1)) {
      const c2 = map1to2(c1);           /* matching corners2 */
      /* The 'x' here refers to corners1, 'y' to corners2. */
      sumx += corners1(c1).center(cbase);
      sumv += corners2(c2).center(cbase);
      sumxv += corners1(c1).center(cbase) * corners2(c2).center(cbase);
      sumx2 += corners1(c1).center(cbase) * corners1(c1).center(cbase);
      nmap += 1;
    }
  }

  m = (nmap * sumxv - sumx * sumv) : real / (nmap * sumx2 - sumx * sumx);
  b = (sumv - m * sumx) / nmap;
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
    quadrant:  Determine which quadrant of the plane (1 is x, y positive going
               ccw) a corner lies in compared to a base.
    args:      corner1 - base corner, anchor of comparison
               corner2 - placed corner
    returns:   1 t/m 4
***/
inline proc quadrant(corner1 : corner, corner2 : corner) : int {
  const (xc, yc) = corner1.center;      /* corner point of corner1 */
  const (vc, wc) = corner2.center;      /* corner point of corner2 */

  if (yc <= wc) {
    if (xc <= vc) then return 1;
    else return 2;
  } else {
    if (xc <= vc) then return 4;
    else return 3;
  }
}


/***
    random_ranged:  Return a random number in the range, incl.  Assumes integer
                    type.
    args:           rand - random number stream
                    rng - range to get random number within
    returns:   random point within range
***/
proc random_ranged(ref rand : randomStream(?), rng : range) : rng.idxType {
  const elt = rng.size * rand.getNext();      /* scale random to range */

  /* It would be nice to have a method on ranges that returns the i'th
     value.  That way we wouldn't have to worry about the type. */
  return rng.first + rng.stride * (nearbyint(elt - 0.5) : rng.idxType);
}

/***
    random_bound:  Return a number in the range, incl.
    args:          rand - random number generator
                   rmin, rmax - bounds of range (incl.)
    returns:   random number from rmin t/m rmax
***/
proc random_bound(ref rand : randomStream(?), rmin : int, rmax : int) : int {
  const len = (rmax - rmin + 1) : real;
  const elt = len * rand.getNext();             /* scale random to range */
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
  var corners1 : unmanaged chunkarray(corner)?;   /* corners found in clr1 */
  var corners2 : unmanaged chunkarray(corner)?;   /* corners found in clr2 */
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

  writeln("\nBest alignment");
  writef("  x2 = %6.3dr x1 %+7.1dr\n", bestmap.sx, bestmap.dx);
  writef("  y2 = %6.3dr y1 %+7.1dr\n", bestmap.sy, bestmap.dy);
  writef("\n");

  free_rgbimage(rgb);
  delete clr1;
  delete clr2;
  delete corners1;
  delete corners2;

  return 0;
}
