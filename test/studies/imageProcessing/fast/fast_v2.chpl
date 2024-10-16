
/*****
      fast_v2.chpl -
      Perform a FAST corner detection on an image.  Uses a custom iterator for
      the circle for each FAST operator.
      This version suppresses as many of the corners as it can by picking the
      strongest within a region of the other.

      Call:
        fast_v2
          --inname1=<file>         input file
          --outname=<file>         greyscale input marked with corners
          --space=[LAB|YUV]        color space to use for greyscale generation
          --radius=#               radius of FAST circle
          --minlen=#               shortest consecutive different pixels
          --maxlen=#               longest consecutive different pixels
          --thr=#                  needed greyscale difference from center
                                   note different ranges for L, Y
          --suppsep=#              x, y difference to suppress one corner

      c 2015-2018 Primordial Machine Vision Systems
*****/

public use ip_color_v3;
use Sort;
use Help;
use CTypes;
use Math;


/**** Command Line Arguments ****/

config const inname : string;           /* input file name */
config const outname : string;          /* output with corners marked */
config const space = clrspace.LAB;      /* color space to work in */
config const radius = 3;                /* radius circle for FAST analysis */
config const minlen = 10;               /* shortest FAST run at corner */
config const maxlen = 13;               /* longest consecutive run at corner */
config const thr = 20.0;                /* threshold pixel differing center */
config const suppsep = 5;               /* x, y diff b/t corners to suppress */


/**** Data Structures ****/

/* requirements for FAST detection */
record fastspec {
  var radius : int;                     /* radius circle for FAST analysis */
  var minlen : real;                    /* shortest consecutive string */
  var maxlen : real;                    /* longest consecutive string */
  var thr : real;                       /* greyscale diff from center pixel */
}

record corner {
  var xc, yc : int;                     /* corner point */
  var len : int;                        /* length of background sequence */
  var st : 2 * int;                     /* first point of bg sequence */
  var dir : thrdir;                     /* relationship bg to center point */
  var dpix : real;                      /* max diff in sequence to center */
}

/* whether intensity difference passes threshold and if so in which direction
     SAME - intensities within threshold
     MORE - pixel differs from center by more than threshold
     LESS - pixel differs from center by less than threshold
*/
enum thrdir {
  SAME, MORE, LESS
};


/**** Iterator ****/

/* We wrap the iterator in a class so we can cache the quadrant.  As long
   as the radius doesn't change (if you're iterating over an image) you
   won't need to re-establish the pattern for each use.

   To use the class as a one-shot iterator you can do
     for (x, y) in new circumference(r, xc, yc, [closed]) do ...
   where r is the radius, xc, yc are the center of the circle, and <closed>
   is an optional flag to repeat the first point at the end, ie. to close the
   circle.

   To create this once for a given radius do ...
     var circle = new circumerence(r, [closed]);
   and use it as an iterator giving the center as an argument
     for (x, y) in circle(xc, yc, [closed]) do ...
   (The trick we have to do turning the 'this' method into 'these' is probably
   evil.)
   You can change the radius by calling the instance with just one argument.
     circle(newr);

   Public methods:
     circumference - constructors, two versions
                     one-time use takes radius and center
                     re-usable takes only radius
     these - standard iterator interface, one-time use, not parallel safe
     this - re-usable call on instace to place ring at point, start iteration
*/
class circumference {
  var r : int;                          /* radius of circle */
  var Lquad : domain(1);                /* array size */
  var quad : [Lquad] 2 * int;           /* points of first quadrant */
  var quadcnt : int;                    /* max index in quad (incl.) */
  /* These members are used for the standalone version created with new
     above.  We need to hold them for the built-in iterator method that
     takes no arguments. */
  var x_center, y_center : int;         /* center of circle */
  var close_circle : bool;              /* true to repeat first point at end */

  /***
      init:           Initializer that raises an error - you must
                      use the one with a radius.
  ***/
  proc init() {
    writeln("you must provide at least a radius");
    halt();
  }

  /** Stand-alone mode **/

  /***
      init:           Initializer that prepares the iterator to generate a
                      circle of a given radius.  Also sets up the center and
                      optionally whether to close the the circle when looping.
      args:           radius - radius of circle (>= 3)
                      xc, yc - center of circle
                      closed - true to dup first point at end
      modifies:  r, x_center, y_center, close_circle
  ***/
  proc init(radius : int, xc : int, yc : int, closed = false) {

    x_center = xc;
    y_center = yc;
    close_circle = closed;
    init this;
    set_radius(radius);
  }

  /**!!!**/
  /* Is there a cleaner way to do this?  It repeats the code of the version
     that takes arguments.  But you can't call that from here, ie.
        iter these() {
          these(x_center, y_center, close_circle);
        }
     because the compiler complains that there's no yield statement, and
     you can't return the value.   So this copy hard-wires the members in,
     while the cached mode uses its arguments and can be run in parallel.
  */

  /***
      these:  Standard iteration interface.  You should never have to call
              this method yourself.  Uses the stored values for the center
              point and closure flag.
  ***/
  iter these() {
    /* members copied in to variables used in circum_iter, except for these
       three lines the rest of the procedure duplicates that. */
    const xc = x_center;
    const yc = y_center;
    const closed = close_circle;

    for i in 0..quadcnt {
      var (x, y) = quad(i);
      yield (xc + x, yc + y);
    }
    /* - 1 because we don't want to duplicate the last point of
       previous quadrant. */
    for i in 0..quadcnt-1 by -1 {
      var (x, y) = quad(i);
      yield (xc - x, yc + y);
    }
    for i in 1..quadcnt {
      var (x, y) = quad(i);
      yield (xc - x, yc - y);
    }
    for i in 1..quadcnt-1 by -1 {
      var (x, y) = quad(i);
      yield (xc + x, yc - y);
    }
    if (closed) {
      var (x, y) = quad(0);
      yield (xc + x, yc + y);
    }
  }

  /** Cached mode **/

  /***
      init:           Initializer that prepares the iterator to generate a
                      circle of a given radius.
      args:           radius - radius of circle (>= 3)
      modifies:  r
  ***/
  proc init(radius : int) {

    init this;
    set_radius(radius);
  }

  /***
      this:  Change the center of the circle and begin to iterate around the
             periphery.  Yes, this is probably an evil abuse of 'this', but
             it's a clean way to use an instance in a loop.
      args:  center_x, center_y - center of circle
             closed_circle - true to repeat the first point at the end
      modifies:  xc, yc, closed
  ***/
  proc this(xc : int, yc : int, closed = false) {

    return circum_iter(xc, yc, closed);
  }

  /** Internal operation (private) **/

  /***
      circum_iter:  Internal iteration that does the actual work.
      args:         xc, yc - center of circle
                    closed - bool
  ***/
  iter circum_iter(xc : int, yc : int, closed : bool) {

    /* Note code duplicated in these(). */
    for i in 0..quadcnt {
      var (x, y) = quad(i);
      yield (xc + x, yc + y);
    }
    /* - 1 because we don't want to duplicate the last point of
       previous quadrant. */
    for i in 0..quadcnt-1 by -1 {
      var (x, y) = quad(i);
      yield (xc - x, yc + y);
    }
    for i in 1..quadcnt {
      var (x, y) = quad(i);
      yield (xc - x, yc - y);
    }
    for i in 1..quadcnt-1 by -1 {
      var (x, y) = quad(i);
      yield (xc + x, yc - y);
    }
    if (closed) {
      var (x, y) = quad(0);
      yield (xc + x, yc + y);
    }
  }

  /** Setup (private) **/

  /***
      set_radius:  Change the radius of the circle, updating the cache if
                   needed.  Note that you don't call the 'this' method, just
                   pass a single integer to the class instance.
      args:  radius - radius of circle (>= 3)
      modifies:  r
  ***/
  proc set_radius(radius : int) {

    if (radius < 3) {
      writeln("circle circumference must have radius >= 3, not ", radius);
      halt();
    } else {
      if (r != radius) {
        r = radius;
        prep_quadrant();
      }
    }
  }

  /***
      prep_quadrant:  Cache the circumferntial points in the first quadrant.
                      Assumes the radius r has already been set and is >= 3.
      modifies:  quad, quadcnt, quadrng;
  ***/
  proc prep_quadrant() {

    /* This is a little too big, but we're guaranteed not to have more than
       this many, and calculating the exact number beforehand is a bother. */
    Lquad = 0..2*r;

    /*   r=3        r=4          r=5            r=6              r=7       */
    /*                                                                     */
    /*                                                          *****      */
    /*                                         *****          **.....**    */
    /*                          *****         *.....*        *.........*   */
    /*              ***        *.....*       *.......*      *...........*  */
    /*   ***       *...*      *.......*     *.........*     *...........*  */
    /*  *...*     *.....*    *.........*   *...........*   *.............* */
    /* *.....*   *.......*   *.........*   *...........*   *.............* */
    /* *..c..*   *...c...*   *....c....*   *.....c.....*   *......c......* */
    /* *.....*   *.......*   *.........*   *...........*   *.............* */
    /*  *...*     *.....*    *.........*   *...........*   *.............* */
    /*   ***       *...*      *.......*     *.........*     *...........*  */
    /*              ***        *.....*       *.......*      *...........*  */
    /*                          *****         *.....*        *.........*   */
    /*                                         *****          **.....**    */
    /*                                                          *****      */

    if (3 == r) {
      quad(0) = (3, 0);
      quad(1) = (3, 1);
      quad(2) = (2, 2);
      quad(3) = (1, 3);
      quad(4) = (0, 3);
      quadcnt = 4;
    } else if (4 == r) {
      quad(0) = (4, 0);
      quad(1) = (4, 1);
      quad(2) = (3, 2);
      quad(3) = (2, 3);
      quad(4) = (1, 4);
      quad(5) = (0, 4);
      quadcnt = 5;
    } else if (5 == r) {
      quad(0) = (5, 0);
      quad(1) = (5, 1);
      quad(2) = (5, 2);
      quad(3) = (4, 3);
      quad(4) = (3, 4);
      quad(5) = (2, 5);
      quad(6) = (1, 5);
      quad(7) = (0, 5);
      quadcnt = 7;
    } else if (6 == r) {
      quad(0) = (6, 0);
      quad(1) = (6, 1);
      quad(2) = (6, 2);
      quad(3) = (5, 3);
      quad(4) = (4, 4);
      quad(5) = (3, 5);
      quad(6) = (2, 6);
      quad(7) = (1, 6);
      quad(8) = (0, 6);
      quadcnt = 8;
    } else if (7 == r) {
      quad(0) = (7, 0);
      quad(1) = (7, 1);
      quad(2) = (7, 2);
      quad(3) = (6, 3);
      quad(4) = (6, 4);
      quad(5) = (5, 5);
      quad(6) = (4, 6);
      quad(7) = (3, 6);
      quad(8) = (2, 7);
      quad(9) = (1, 7);
      quad(10) = (0, 7);
      quadcnt = 10;
    } else {
      var mid = 0;                      /* start of second half of quadrant */
      var x, y : int;                   /* circum point */

      quadcnt = 0;
      x = r;
      y = 0;
      do {
        quad(quadcnt) = (x, y);
        quadcnt += 1;
        y += 1;
        x = nearbyint(sqrt(r**2 - y**2)) : int;
      } while (y < x);
      do {
        y = nearbyint(sqrt(r**2 - x**2)) : int;
        quad(quadcnt) = (x, y);
        quadcnt += 1;
        x -= 1;
      } while (0 <= x);
      quadcnt -= 1;
    }
  }
}



/**** Chunked Array ****/

/* A simplified version of an ArrayList that over-allocates the array size
   so that we can add elements with fewer size changes (which are expensive).
   The data is available via the range Ldata, which is a sub-range of the
   complete array that's been allocated.  The only operation we need to support
   for this application is adding elements.  You can address an instance like
   an array, or iterate over an instance normally.  When instantiating the
   class provide the type of the element you'll be storing.

   Public methods:
     this - with argument, returns array element at index
            without argument, returns a copy of the stored elements
     these - iterator over data
     append - add an element to the array (re-sizes array if needed)
     size - return number of data points stored
*/
class chunkarray {
  type eltType;                         /* generic array */
  param INIT_ALLOC                      /* initial allocation size */
    = 1000;
  param GROW_ALLOC                      /* chunk/allocation size */
    = 500;
  var Lalloc : domain(rank=1)           /* internal allocation range */
    = 1..INIT_ALLOC;
  var Ldata : domain(rank=1)            /* range for data (starts empty) */
    = 1..0;
  var data : [Lalloc] eltType;          /* the allocation */
  var lock : sync int                   /* synchronization lock in append */
    = 1;

  /***
      init:  Default initializer.
  ***/
  proc init(type elemType) {
    eltType = elemType;
  }

  /***
      this:  Return a copy of the data array limiting only to stored entries.
  ***/
  proc this() {
    return data[Ldata];
  }

  /***
      this:  Retrieve an element at an index.
      args:  ind - position in array to retrieve
      returns:  value stored at ind
  ***/
  proc this(ind : int) : eltType {
    return data(ind);
  }

  /***
      these:  Iterator across the data that's been stored.
  ***/
  iter these() {
    for d in data[Ldata] do
      yield d;
  }

  /***
      append:  Add an element to the end of the array.
      args:    val - element to add
      modifies:  data, Lalloc, Ldata
  ***/
  proc append(const in val : eltType) {
    lock.readFE();

    /* Slice the expansion to trim negative values. */
    if (Ldata.high == Lalloc.high) {
      Lalloc = Lalloc.expand(GROW_ALLOC)[1..];
    }
    Ldata = Ldata.expand(1)[1..];
    data(Ldata.high) = val;

    lock.writeEF(1);
  }

  /***
      size:  Return the actual number of elements stored in the array.
  ***/
  proc size() {
    return Ldata.high;
  }
}



/**** FAST Detector ****/

/***
    find_corners:  Build a list of all corners in an image.  Then remove the
                   weakest (with the smallest gap between the center pixel
                   and the continuous ring segment) that are within range of
                   a stronger.  The result is sorted first in x, then in y.
    args:          img - greyscale image (in c1 plane) to look for corners
                   spec - parameters for FAST detector
    returns:   list of corners (may be empty)
***/
proc find_corners(img : unmanaged clrimage?, spec : fastspec) : unmanaged chunkarray(corner) {
  const circle                          /* iterator about pixel */
    = new unmanaged circumference(spec.radius);
  const Ainside                         /* image interior we can analyze */
    = img!.area.expand(-spec.radius, -spec.radius);
  var corners                           /* corners we've found */
    = new unmanaged chunkarray(corner);

  forall (y,x) in Ainside {
    var details : corner;               /* info about corner */
    if (is_corner_with_details(img, x, y, spec, circle, details)) {
      corners.append(details);
    }
  }

  writef("\nFAST analysis with r=%i, minlen=%i, maxlen=%i, thr=%5.1dr\n",
         spec.radius, spec.minlen, spec.maxlen, spec.thr);
  writeln("  found ", corners.size(), " corners");

  var retval                            /* stash result while we clean up */
    = suppress_corners(corners);

  delete circle;
  delete corners;
  return retval;
}

/***
    is_corner:  Use the FAST corner detector at a point in an image.
    args:       img - greyscale image, uses plane C1
                x, y - pixel to test
                spec - requirements for FAST analysis
                circle - iterator with radius pre-set
    returns:   true if passes FAST criteria, false if not
***/
proc is_corner(img : unmanaged clrimage?, x : int, y : int, spec : fastspec,
               circle : circumference) : bool {
  var details : corner;                 /* temp storage (not used) */

  return is_corner_with_details(img, x, y, spec, circle, details);
}

/***
    is_corner_with_details:  Use the FAST corner detector at a point in an
                             image, returning details about the match if there
                             is one.
    args:                    img - greyscale image, uses plane C1
                             x, y - pixel to test
                             spec - requirements for FAST analysis
                             circle - iterator with radius pre-set
                             details - description of FAST match
    returns:   true if passes FAST criteria, false if not
    modifies:  details (may be filled with garbage if returns false)
***/
proc is_corner_with_details(img : unmanaged clrimage?, x : int, y : int, spec : fastspec,
                            circle : circumference, out details : corner) : bool {
  var dir : thrdir;                     /* pixel difference direction */
  var len : int;                        /* consecutive length same dir */
  var prevdir : thrdir;                 /* last different direction */
  var stpt : 2 * int;                   /* first point in sequence */
  var dpix : real;                      /* max diff to center pixel in seq */
  var initlen : int;                    /* length at start of iteration */
  var initdir : thrdir;                 /* direction at iteration start */
  var initpt : 2 * int;                 /* start of first sequence */
  var initdpix : real;                  /* max diff to center at start */
  var first : bool;                     /* special handling first pixel */

  dir = thrdir.SAME;
  len = 0;
  prevdir = thrdir.SAME;
  initlen = 0;
  initdir = thrdir.SAME;
  first = true;

  for (xp, yp) in circle(x, y, false) {
    dir = pixel_thrdir(img, x, y, xp, yp, spec);
    if (first) {
      prevdir = dir;
      len = 1;
      stpt = (xp, yp);
      initpt = (xp, yp);
      dpix = abs(img!.c1(yp, xp) - img!.c1(y, x));
      first = false;
    } else if (prevdir != dir) {
      /* We can't claim victory if this is the first sequence because it
         might wrap at the first point (initlen condition). */
      if ((spec.minlen <= len) && (len <= spec.maxlen) && (0 < initlen) &&
          (thrdir.SAME != prevdir)) {
        register_details(x, y, len, stpt, prevdir, dpix, details);
        return true;
      }
      if (0 == initlen) {
        initlen = len;
        initdir= prevdir;
        initpt = stpt;
        initdpix = dpix;
      }
      len = 1;
      stpt = (xp, yp);
      dpix = abs(img!.c1(yp, xp) - img!.c1(y, x));
      prevdir = dir;
    } else if (thrdir.SAME != dir) {
      len += 1;
      dpix = abs(img!.c1(yp, xp) - img!.c1(y, x));
    }
  }

  /* Check for wraps; if not, then this last piece or what we had at the
     start may also be good. */
  if (initdir == dir) {
    len += initlen;
    if ((spec.minlen <= len) && (len <= spec.maxlen) && (thrdir.SAME != dir)) {
      if (((thrdir.MORE == dir) && (initdpix < dpix)) ||
          ((thrdir.LESS == dir) && (dpix < initdpix))) {
        register_details(x, y, len, stpt, dir, dpix, details);
      } else {
        register_details(x, y, len, stpt, dir, initdpix, details);
      }
      return true;
    }
  } else if ((spec.minlen <= len) && (len <= spec.maxlen) &&
             (thrdir.SAME != dir)) {
    register_details(x, y, len, stpt, dir, dpix, details);
    return true;
  } else if ((spec.minlen <= initlen) && (initlen <= spec.maxlen) &&
             (thrdir.SAME != initdir)) {
    register_details(x, y, initlen, initpt, initdir, initdpix, details);
    return true;
  }

  return false;
}

/***
    pixel_thrdir:  Compare the greyscale value at pt2 to pt1, classifying it
                   above/below the threshold.
    args:          img - greyscale image, uses plane C1
                   x1, y1 - base point
                   x2, y2 - comparison point (direction relative to pt1)
                   spec - requirements for FAST detector
    returns:   thrdir classification at pt2
***/
inline proc pixel_thrdir(img : unmanaged clrimage?, x1 : int, y1 : int,
                         x2 : int, y2 : int, spec : fastspec) : thrdir {

  if (img!.c1(y2,x2) + spec.thr <= img!.c1(y1,x1)) then return thrdir.LESS;
  else if (img!.c1(y1,x1) + spec.thr <= img!.c1(y2,x2)) then return thrdir.MORE;
  else return thrdir.SAME;
}

/***
    register_details:  Copy the corner details into the summary data
                       structure.
    args:              xc, yc - corner point
                       len - length of continuously differing pixels
                       stpt - (x,y) tuple of first differing pixel
                       dir - direction of difference, MORE or LESS
                       dpix - biggest difference b/t differing pixel and center
                       details - where to store info
    modifies:  details
***/
proc register_details(xc : int, yc : int, len : int, stpt : 2*int,
                      dir : thrdir, dpix : real, out details : corner) {

  details.xc = xc;
  details.yc = yc;
  details.len = len;
  details.st = stpt;
  details.dir = dir;
  details.dpix = dpix;
}

/***
    suppress_corners:  Remove corners from the list that are close to another
                       and have a smaller difference between the center pixel
                       and the ring segment.  The result is sorted first in x
                       then in y.
    args:              rawcnr - list of corners from FAST
    returns:   new list of corners kept
    modifies:  rawcnr (list sorted)
***/
proc suppress_corners(rawcnr : chunkarray(corner)) : unmanaged chunkarray(corner) {
  var cnr                               /* actual data in rawcnr */
    = rawcnr();
  var suppcnr                           /* corners to keep */
    = new unmanaged chunkarray(corner);
  var parent : [rawcnr.Ldata] int;      /* parent node in forest */

  /* Comparison function for sort, first by center x, then y. */
  proc DefaultComparator.compare(c1 : corner, c2 : corner) : int {
    if (c1.xc == c2.xc) then return (c1.yc - c2.yc);
    else return (c1.xc - c2.xc);
  }

  sort(cnr);

  /* Remember, sets all elements to -1. */
  parent = -1;

  for c1 in cnr.domain {
    for c2 in cnr.domain[c1+1..] {
      /* Sorted by x, so we don't have to check c2.xc < c1.xc. */
      if ((cnr(c1).xc + suppsep) < cnr(c2).xc) then break;
      if (abs(cnr(c1).yc - cnr(c2).yc) <= suppsep) {
        set_parent(cnr, c2, c1, parent);
      }
    }
  }

  for c1 in cnr.domain do
    if (parent(c1) < 0) then suppcnr.append(cnr(c1));

  writeln("  ", suppcnr.size(), " remain after suppression\n");

  return suppcnr;
}

/***
    set_parent:  Ensure the nodes in the forest have the right relationship:
                 both are under one parent with the biggest rising to the top.
    args:        corners - details about corners
                 c1, c2 - indices of corners to set relationship
                 parent - parent nodes in forest
    modifies:  parent
***/
proc set_parent(corners : [] corner, c1 : int, c2 : int, ref parent : [] int) {
  var root1 = c1;                     /* search up c1 tree to top */
  while (0 <= parent(root1)) do {
    root1 = parent(root1);
  }

  var root2 = c2;                     /* search up c2 tree to top */
  while (0 <= parent(root2)) do {
    root2 = parent(root2);
  }

  if (root1 != root2) {
    if (corners(root1).dpix <= corners(root2).dpix) {
      parent(root1) = root2;
    } else {
      parent(root2) = root1;
    }
  }
}

/***
    mark_corners:  Plot the corners given on a greyscale copy of the input
                   using a colored pixel, so save all planes of the image
                   when done.
    args:          clr - greyscale image (LAB or YUV plane)
                   space - which color space we're using
                   corners - array with details of corners to plot
                   marked - greyscale copy of clr with corners marked in color
    returns:   0 if successful
               < 0 on failure (value depends on error)
    modifies:  marked
***/
proc mark_corners(clr : unmanaged clrimage?, space : clrspace,
                  corners : unmanaged chunkarray(corner)?, ref marked : c_ptr(rgbimage)) : int {
  var retval : int;

  retval = alloc_rgbimage(marked, clr!.ncol : c_int, clr!.nrow : c_int);
  if (retval < 0) then return retval;

  forall (y, x) in clr!.area {
    const xy = y * marked.deref().ncol + x;     /* pixel index */
    /* L from 0 t/m 100.  Y from 16 t/m 235, don't scale. */
    if (clrspace.LAB == space) {
      marked.deref().r(xy) = nearbyint(2.55 * clr!.c1(y,x)) : c_uchar;
    } else if (clrspace.YUV == space) {
      marked.deref().r(xy) = nearbyint(clr!.c1(y,x)) : c_uchar;
    }
    marked.deref().g(xy) = marked.deref().r(xy);
    marked.deref().b(xy) = marked.deref().r(xy);
  }

  /* Just marking the point. */
  /*
  forall c in corners.Ldata {
    const xy = corners(c).yc * marked.ncol + corners(c).xc;
    marked.r(xy) = 255;
    marked.g(xy) = 0;
    marked.b(xy) = 0;
  }
  */

  /* Making a small cross at the corner. */
  forall c in corners!.Ldata {
    for y in clr!.rows[corners!(c).yc-5..corners!(c).yc+5] {
      const xy = y * marked.deref().ncol + corners!(c).xc;
      marked.deref().r(xy) = 255;
      marked.deref().g(xy) = 0;
      marked.deref().b(xy) = 0;
    }
    for x in clr!.cols[corners!(c).xc-5..corners!(c).xc+5] {
      const xy = corners!(c).yc * marked.deref().ncol + x;
      marked.deref().r(xy) = 255;
      marked.deref().g(xy) = 0;
      marked.deref().b(xy) = 0;
    }
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
  if ((clrspace.LAB != space) && (clrspace.YUV != space)) then
    usage("only use LAB or YUV color spaces");
  if (radius < 3) then usage("radius too small");
}

proc main() {
  var rgb : c_ptr(rgbimage);            /* image we've read */
  var clr : unmanaged clrimage?;        /* converted image with greyscale */
  var marked : c_ptr(rgbimage);         /* quantized color assignments */
  var spec : fastspec;                  /* FAST parameters */
  var corners : unmanaged chunkarray(corner)?;     /* corners found in image */
  var retval : int;

  verify_setup();

  retval = PNG_read(inname.c_str(), rgb);
  end_onerr(retval, rgb);

  rgb_convert(rgb, clr, space);

  spec.radius = radius;
  spec.minlen = minlen;
  spec.maxlen = maxlen;
  spec.thr = thr;

  corners = find_corners(clr, spec);

  retval = mark_corners(clr, space, corners, marked);
  end_onerr(retval, rgb, marked, clr, corners);

  retval = PNG_write(outname.c_str(), marked, CLR_RGB);
  end_onerr(retval, rgb, marked, clr, corners);

  free_rgbimage(rgb);
  free_rgbimage(marked);
  delete clr;
  delete corners;

  return 0;
}
