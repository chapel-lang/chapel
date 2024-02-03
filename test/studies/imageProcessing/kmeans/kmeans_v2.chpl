
/*****
      kmeans_.chpl -
      Quantize colors in an image by using k-means clustering.  Not all
      clusters may be used (some may end up empty).  Prints the color of
      each cluster to stdout.
      This version uses atomic variables in the cluster for the iteration pass.

      Call:
        kmeans
          --inname=<file>              input file
          --outname=<file>             8-bit image with cluster assignments
          --space=[LAB|YUV|HSV|RGB]    color space to work in
          --ncluster=<int>             number of clusters to seed

      c 2015-2018 Primordial Machine Vision Systems
*****/

use ip_color_v3;
use Math;
use Sort;
use Random;
use Time;
use Help;
use CTypes;


/**** Command Line Arguments ****/

config const inname : string;           /* input file name */
config const outname : string;          /* where to write ID assignments */
config const space = clrspace.LAB;      /* color space to work in */
config const ncluster = 50;             /* number of clusters to seed */


/**** Constants for Analysis ****/

const DSEED = 0.2;                      /* minimum separation b/t seeds */
const NPTSEED = 500;                    /* number points to try each seed */
const MAX_DNPIX = 0.015;                /* max change pop ends clustering */
const MAX_NPASS = 150;                  /* number passes to converge */


/**** Data Structures ****/

record cluster {
  var skip : bool;                      /* true to ignore cluster in set */
  var npix : int;                       /* number of pixels in cluster */
  var c1, c2, c3 : real;                /* center of mass each color plane */
  var sumpix : atomic int;              /* incremental sum for num pixels */
  var sum1, sum2, sum3 : atomic real;   /* sums for centers of mass */
  var sumh : atomic real;               /* temp needed to sum H */
  var r, g, b : c_uchar;                /* equivalent RGB value of cluster */
}


/**** Support ****/

/***
    random_ranged:  Return a random number in the range, incl.  Assumes integer
                    type.
    args:           rand - random number stream
                    rng - range to get random number within
    returns:   random point within range
***/
proc random_ranged(rand : RandomStream, rng : range) : rng.idxType {
  const elt = rng.size * rand.getNext();      /* scale random to range */

  /* It would be nice to have a method on ranges that returns the i'th
     value.  That way we wouldn't have to worry about the type. */
  return rng.first + rng.stride * (nearbyint(elt - 0.5) : rng.idxType);
}

/***
    random_domain:  Return a tuple of a random point in the domain.  Assumes
                    integer types for indices.
    args:           rand - random number stream
                    dom - domain to generate point in
    returns:   random point within domain
***/
proc random_domain(rand : RandomStream, dom : domain) : dom.rank * dom.idxType {
  /* Do not use dom.numIndices.  dom.rank is a param, and the tuple declaration
     requires a parameter. */
  var pt : dom.rank * dom.idxType;

  for i in 0..(dom.rank-1) do pt(i) = random_ranged(rand, dom.dim(i));
  return pt;
}


/**** K-Means Clustering ****/

/***
    cluster_colors:  Perform a K-means clustering on the input image, already
                     split into color planes.  Builds an 8-bit image with the
                     cluster ID assignments, beginning at 1.  The colors for
                     each ID are the average values in the clusters.
    args:            clr - color image to quantize
                     ncluster - number of clusters
                     kset - clusters we've found
                     quant - 8-bit image with IDs assigned in CLR_R plane
    returns:   0 if successful
               < 0 on failure (value depends on error)
    modifies:  kset, ids
***/
proc cluster_colors(clr : unmanaged clrimage?, ncluster : int,
                    out kset : [] cluster, ref quant : c_ptr(rgbimage)) : int {
  const ids : domain(1) = { 1..ncluster };
                                        /* range of clusters */
  var kprev : [ids] cluster;            /* clusters in previous pass */
  var kcurr : [ids] cluster;            /* clusters this pass */
  var dnpix : real;                     /* fractional change cluster pop */
  var maxdnpix : real;                  /* biggest fractional change pop */
  var npass : int;                      /* number passes we've made */
  var t0, t1 : stopwatch;               /* track time of execution */
  var retval : int;

  t0.start();

  seed_clusters(clr, kprev);

  do {
    kcurr = iterate_pass(clr, kprev);

    fillin_empties(kcurr, clr!.space);

    maxdnpix = min(real);
    for k in ids do
      if (!kcurr(k).skip) {
        dnpix = abs((kcurr(k).npix - kprev(k).npix) : real / kcurr(k).npix);
        if (maxdnpix < dnpix) {
          maxdnpix = dnpix;
        }
      }

    if (maxdnpix == min(real)) {
      writeln("no pixels assigned to clusters");
      return -1;
    }

    writef("pass %3i   max dnpix %5.3dr\n", npass, maxdnpix);
    npass += 1;
    kprev = kcurr;
  } while ((MAX_DNPIX < maxdnpix) && (npass < MAX_NPASS));

  if ((MAX_DNPIX < maxdnpix) && (MAX_NPASS == npass)) {
    writeln("\nhit maximum limit of clustering passes - did not converge");
  }

  kset = kcurr;

  t0.stop();
  t1.start();

  retval = mark_and_backfill(clr, kset, quant);

  t1.stop();

  writeln("\nTiming (in ms)\n");
  writef("  color clustering   %8.3dr\n", t0.elapsed() * 1_000);
  writef("  color mapping      %8.3dr\n", t1.elapsed() * 1_000);

  return retval;
}

/***
    iterate_pass:  Perform one iteration of the clustering.  Assign pixels
                   in each image to the nearest cluster and calculate the
                   new average (per color plane) for each cluster when done.
    args:          clr - color image
                   kset - cluster definitions at start of pass
    returns:   new set of clusters
***/
proc iterate_pass(clr : unmanaged clrimage?, const ref kset : [] cluster) {
  var knew : [kset.domain] cluster;     /* new cluster definitions */

  forall (y, x) in clr!.area {
    const closest =
      closest_cluster(clr!.c1(y,x), clr!.c2(y,x), clr!.c3(y,x), clr!.space, kset);
    knew(closest).sumpix.add(1);
    if (clr!.space == clrspace.HSV) {
      knew(closest).sum1.add(sin(pi * clr!.c1(y,x) / 180.0));
      knew(closest).sumh.add(cos(pi * clr!.c1(y,x) / 180.0));
    } else {
      knew(closest).sum1.add(clr!.c1(y,x));
    }
    knew(closest).sum2.add(clr!.c2(y,x));
    knew(closest).sum3.add(clr!.c3(y,x));
  }

  forall k in knew {
    k.npix = k.sumpix.read();
    if (0 == k.npix) {
      k.skip = true;
    } else {
      if (clr!.space == clrspace.HSV) {
        k.c1 = atan2(k.sum1.read(), k.sumh.read());
      } else {
        k.c1 = k.sum1.read() / k.npix;
      }
      k.c2 = k.sum2.read() / k.npix;
      k.c3 = k.sum3.read() / k.npix;
    }
  }

  return knew;
}

/***
    cluster_sep:  The metric we use for grouping.  This is the difference
                  between the pixel's color and the cluster average.  The
                  ranges for the color spaces are scaled to 0-1.0 so that
                  all three planes contribute evenly.
    args:         pixc1, pixc2, pixc3 - pixel color values
                  space - color space of pixel
                  clustc1, clustc2, clustc3 - cluster color value (COM)
    returns:   distance metric
***/
proc cluster_sep(pixc1 : real, pixc2 : real, pixc3 : real, space : clrspace,
                 clustc1 : real, clustc2 : real, clustc3 : real) : real {
  var dc1, dc2, dc3 : real;             /* distance in each plane */

  select space {
    when clrspace.HSV {
      /* Need to treat HSV separately because H difference wraps to 0-180.
         S and V don't need scaling. */
      dc1 = pixc1 - clustc1;
      if (dc1 <= -180.0) {
        dc1 += 360.0;
      } else if (180.0 < dc1) {
        dc1 -= 360.0;
      }
      dc1 /= 180.0;
      dc1 **= 2;
      dc2 = (pixc2 - clustc2) ** 2;
      dc3 = (pixc3 - clustc3) ** 2;
    }
    when clrspace.YUV {
      /* Scaling to full range from Y 16-235, U and V 16-239. */
      dc1 = ((pixc1 - clustc1) / (235 - 16)) ** 2;
      dc2 = ((pixc2 - clustc2) / (239 - 16)) ** 2;
      dc3 = ((pixc3 - clustc3) / (239 - 16)) ** 2;
    }
    when clrspace.LAB {
      /* Scaling based on LAB_* values in ip_color. */
      dc1 = ((pixc1 - clustc1) / (LAB_LMAX - LAB_LMIN)) ** 2;
      dc2 = ((pixc2 - clustc2) / (LAB_AMAX - LAB_AMIN)) ** 2;
      dc3 = ((pixc3 - clustc3) / (LAB_BMAX - LAB_BMIN)) ** 2;
    }
    when clrspace.RGB {
      /* 255, natch. */
      dc1 = ((pixc1 - clustc1) / 255.0) ** 2;
      dc2 = ((pixc2 - clustc2) / 255.0) ** 2;
      dc3 = ((pixc3 - clustc3) / 255.0) ** 2;
    }
    otherwise
      return 0.0;
    }

  return (dc1 + dc2 + dc3);
}

/***
    closest_cluster:  Determine which cluster the pixel is closest to.
    args:             c1, c2, c3 - color values at pixel
                      space - color space we're working in
                      kset - all clusters
    returns:   index of closest cluster
***/
proc closest_cluster(c1 : real, c2 : real, c3 : real, space : clrspace,
                     kset : [] cluster) : int {
  var d : real;                         /* distance from point to cluster */
  var dmin : real;                      /* closest distance */
  var closest : cluster;                /* nearest cluster */
  var minind : int;                     /* nearest cluster index */

  dmin = max(real);
  for k in kset.domain do
    if (!kset(k).skip) {
      d = cluster_sep(c1, c2, c3, space, kset(k).c1, kset(k).c2, kset(k).c3);
      if (d < dmin) {
        dmin = d;
        minind = k;
      }
    }

  return minind;
}

/***
    fillin_empties:  For very empty (npix = 0) cluster, re-seed it to
                     halfway between the biggest and next biggest that haven't
                     been used (ie. work down a sorted list).  If there are
                     more empties than pairs of non-empties, then leave them
                     so.  kset is sorted in descending order of pixel
                     counts as a side effect.
    args:            kset - cluster assignments after image pass
                     space - color space data is in
    modifies:  kset
***/
proc fillin_empties(kset : [] cluster, space : clrspace) {
  var kst : int;                        /* first of averaging pair */

  record Comparator {};
  /* Comparison function to sort by the number of pixels in the cluster. */
  proc Comparator.compare(c1 : cluster, c2 : cluster) {
    return (c1.npix - c2.npix);
  }
  var comparator: Comparator;
  var clusterReverseComparator = new ReverseComparator(comparator);
  sort(kset, comparator=clusterReverseComparator);

  kst = 1;
  /* Nothing to do, we've collapsed down to 0 or 1 clusters. */
  if ((0 == kset(kst).npix) || (0 == kset(kst+1).npix)) {
    return;
  }

  for kend in kset.domain by -1 do {
    if (kend <= kst) then break;
    if (0 == kset(kend).npix) {
      kset(kend).skip = false;
      if (clrspace.HSV == space) {
        /* The method used in iterate_pass (convert angles to points on unit
           circle, sum) would also work here, but for two points this is
           just as good. */
        if (180.0 < abs(kset(kst).c1 - kset(kst+1).c1)) {
          kset(kend).c1 = (kset(kst).c1 + kset(kst+1).c1) / 2.0 - 180.0;
          if (kset(kend).c1 < 0.0) {
            kset(kend).c1 += 360.0;
          }
        } else {
          kset(kend).c1 = (kset(kst).c1 + kset(kst+1).c1) / 2.0;
        }
      } else {
        kset(kend).c1 = (kset(kst).c1 + kset(kst+1).c1) / 2.0;
      }
      kset(kend).c2 = (kset(kst).c2 + kset(kst+1).c2) / 2.0;
      kset(kend).c3 = (kset(kst).c3 + kset(kst+1).c3) / 2.0;
      kst += 1;
      if (0 == kset(kst+1).npix) then break;
    }
  }
}

/***
    seed_clusters:  Place random seeds for all clusters.  Seeds will be at
                    one pixel's value and be well-spaced.
    args:           clr - color image to quantize
                    kset - position of clusters to seed
    modifies:  kset
***/
proc seed_clusters(clr : unmanaged clrimage?, kset : [] cluster) {
  const rand = createRandomStream(real);  /* random numbers to pick pixels */
  var dmax : real;                      /* max distance to seeded clusters */
  var d : real;                         /* distance to another cluster */
  /* Note that the type-checking here isn't good.  You can make this any
     sized tuple and program will compile. */
  var pt : 2 * int;                     /* random point in image */
  var maxpt : 2 * int;                  /* point at dmax */

  pt = random_domain(rand, clr!.area);
  kset(1).skip = false;
  kset(1).c1 = clr!.c1(pt);
  kset(1).c2 = clr!.c2(pt);
  kset(1).c3 = clr!.c3(pt);

  /* The seeding strategy here is to generate a fixed number of points and to
     take the furthest from the previous seeds if it meets the minimum
     distance requirement.  This assumes a somewhat uniform distribution of
     points over the space - otherwise we are likely to keep picking points
     near others in color space, and failing to find enough unique.
     Other ideas to improve the seeding: maintain a list of free points in the
     image and when one is selected as a seed, remove all those within the
     min separation of it; determine the convex hull of the space and pick
     points on the exterior. */
  for i in kset.domain[2..] {
    dmax = min(real);
    for pass in 1..NPTSEED {
      pt = random_domain(rand, clr!.area);
      for j in 1..i-1 {
        d = cluster_sep(clr!.c1(pt), clr!.c2(pt), clr!.c3(pt), clr!.space,
                        kset(j).c1, kset(j).c2, kset(j).c3);
        if (dmax < d) {
          dmax = d;
          maxpt = pt;
        }
      }
    }
    if (DSEED <= dmax) {
      kset(i).skip = false;
      kset(i).c1 = clr!.c1(maxpt);
      kset(i).c2 = clr!.c2(maxpt);
      kset(i).c3 = clr!.c3(maxpt);
    } else {
      writef("  could not seed cluster %i with %i attempts, max %6.4dr\n",
             i, NPTSEED, dmax);
      kset(i).skip = true;
    }
  }
}

/***
    mark_and_backfill:  Clean up.  Allocate an RGB image and store the
                        cluster ID of each pixel in its R plane.  Also
                        calculate the RGB value for the cluster.
    args:               clr - source image
                        kset - cluster in image
                        quant - color-quantized cluster assigments (1 t/m 255)
    returns:   0 if successful
               < 0 on failure (value depends on error)
    modifies:  kset, ids
***/
proc mark_and_backfill(clr : unmanaged clrimage?, kset : [] cluster,
                       ref quant : c_ptr(rgbimage)) : int {
  var retval: int;

  /* Back-calculate RGB. */
  for k in kset {
    if (!k.skip) {
      select (clr!.space) {
      when clrspace.LAB do
        labpix_to_rgb(k.c1, k.c2, k.c3, k.r, k.g, k.b);
      when clrspace.YUV do
        yuvpix_to_rgb(k.c1, k.c2, k.c3, k.r, k.g, k.b);
      when clrspace.HSV do
        hsvpix_to_rgb(k.c1, k.c2, k.c3, k.r, k.g, k.b);
      when clrspace.RGB do
        rgbpix_to_rgb(k.c1, k.c2, k.c3, k.r, k.g, k.b);
      otherwise return -1;
      }
    }
  }

  retval = alloc_rgbimage(quant, clr!.ncol : c_int, clr!.nrow : c_int);
  if (retval < 0) then return retval;

  forall (y, x) in clr!.area {
    var xy = (y * quant.deref().ncol) + x;      /* pixel index */
    quant.deref().r(xy) = closest_cluster(clr!.c1(y,x), clr!.c2(y,x), clr!.c3(y,x),
                                  clr!.space, kset) : c_uchar;
  }

  return 0;
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
  writeln("  --space = HSV | LAB | YUV | RGB");
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
  if (ncluster <= 0) then usage("must have positive number of clusters");
  /* Must fit in 8-bit, less 0. */
  if (255 <= ncluster) then usage("too many clusters");
}

proc main() {
  var rgb : c_ptr(rgbimage);            /* image we've read */
  var clr : unmanaged clrimage?;        /* converted image with greyscale */
  var ids : c_ptr(rgbimage);            /* quantized color assignments */
  var kset : [1..ncluster] cluster;     /* result of quantization */
  var retval : int;

  verify_setup();

  retval = PNG_read(inname.c_str(), rgb);
  end_onerr(retval, rgb);

  rgb_convert(rgb, clr, space);

  retval = cluster_colors(clr, ncluster, kset, ids);
  end_onerr(retval, rgb, clr, ids);

  retval = PNG_write(outname.c_str(), ids, CLR_R);
  end_onerr(retval, rgb, clr, ids);

  writeln("\nClusters found");
  writeln(" id       npix        c1       c2       c3        r    g    b");
  for k in kset.domain {
    if (!kset[k].skip) {
      writef("%3i     %6i     %7.2dr  %7.2dr  %7.2dr     %3u  %3u  %3u\n",
             k, kset(k).npix, kset(k).c1, kset(k).c2, kset(k).c3,
             kset(k).r, kset(k).g, kset(k).b);
    }
  }

  free_rgbimage(rgb);
  delete clr;
  free_rgbimage(ids);

  return 0;
}
