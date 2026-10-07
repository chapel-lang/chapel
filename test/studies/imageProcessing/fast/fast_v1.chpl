
/*****
      fast_v1.chpl -
      Perform a FAST corner detection on an image.  Uses a custom iterator for
      the circle for each FAST operator.

      Call:
        fast_v1
          --inname=<file>          input file
          --outname=<file>         greyscale input marked with corners
          --space=[LAB|YUV]        color space to use for greyscale generation
          --radius=#               radius of FAST circle
          --minlen=#               shortest consecutive different pixels
          --maxlen=#               longest consecutive different pixels
          --thr=#                  needed greyscale difference from center
                                   note different ranges for L, Y

      c 2015-2018 Primordial Machine Vision Systems
*****/

public use ip_color_v3;
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


/**** Data Structures ****/

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
      init:  Initializer that raises an error - you must
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
      init:      Initializer that prepares the iterator to generate a
                 circle of a given radius.
      args:      radius - radius of circle (>= 3)
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

  /** Setup (all code below is private) **/

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



/**** FAST Detector ****/

/***
    mark_corners:  Check each pixel of the input.  If it's a corner then
                   mark it on a copy of the greyscale.  The mark is made in
                   color, so save all planes of the image when done.
    args:          clr - greyscale image (LAB or YUV plane)
                   space - which color space we're using
                   marked - greyscale copy of clr with corners marked in red
    returns:   0 if successful
               < 0 on failure (value depends on error)
    modifies:  marked
***/
proc mark_corners(clr : unmanaged clrimage?, space : clrspace,
                  ref marked : c_ptr(rgbimage)) : int {
  const circle                          /* iterator about pixel */
    = new unmanaged circumference(radius);
  const Ainside                         /* image interior we can analyze */
    = clr!.area.expand(-radius, -radius);
  var cnt : sync int;                   /* number corners found */
  var retval : int;

  retval = alloc_rgbimage(marked, clr!.ncol : c_int, clr!.nrow : c_int);
  if (retval < 0) {
    delete circle;
    return retval;
  }

  /* Double loop over area so we can also get the greyscale outside the
     corner checking area. */
  forall (y, x) in clr!.area {
    const xy = y * marked.deref().ncol + x;     /* pixel index */
    if (clrspace.LAB == space) {
      /* L from 0 t/m 100. */
      marked.deref().r(xy) = nearbyint(2.55 * clr!.c1(y,x)) : c_uchar;
    } else if (clrspace.YUV == space) {
      marked.deref().r(xy) = nearbyint(clr!.c1(y,x)) : c_uchar;
    }
    marked.deref().g(xy) = marked.deref().r(xy);
    marked.deref().b(xy) = marked.deref().r(xy);
  }

  cnt.writeXF(0);
  forall (y, x) in Ainside {
    if (is_corner(clr, x, y, circle)) {
      /* Just marking the point.
      const xy = y * marked.ncol + x;   /* pixel index */
      marked.r(xy) = 255;
      marked.g(xy) = 0;
      marked.b(xy) = 0;
      cnt += 1;
      */

      for yc in clr!.rows[y-5..y+5] {
        const xy = yc * marked.deref().ncol + x;
        marked.deref().r(xy) = 255;
        marked.deref().g(xy) = 0;
        marked.deref().b(xy) = 0;
      }
      for xc in clr!.cols[x-5..x+5] {
        const xy = y * marked.deref().ncol + xc;
        marked.deref().r(xy) = 255;
        marked.deref().g(xy) = 0;
        marked.deref().b(xy) = 0;
      }
      cnt.writeXF(cnt.readXX()+1);
    }
  }

  writef("\nFAST analysis with r=%i, minlen=%i, maxlen=%i, thr=%5.1dr in %s\n",
         radius, minlen, maxlen, thr, space:string);
  writeln("  found ", cnt.readFE(), " corners\n");

  delete circle;

  return 0;
}

/***
    is_corner:  Use the FAST corner detector at a point in an image.
    args:       img - greyscale image, uses plane C1
                x, y - pixel to test
                circle - iterator with radius pre-set
    returns:   true if passes FAST criteria, false if not
***/
proc is_corner(img : unmanaged clrimage?, x : int, y : int,
               circle : circumference) : bool {
  var dir : thrdir;                     /* pixel difference direction */
  var len : int;                        /* consecutive length same dir */
  var prevdir : thrdir;                 /* last different direction */
  var stlen : int;                      /* length at start of iteration */
  var stdir : thrdir;                   /* direction at iteration start */
  var first : bool;                     /* special handling first pixel */

  dir = thrdir.SAME;
  len = 0;
  stlen = 0;
  stdir = thrdir.SAME;
  first = true;

  for (xp, yp) in circle(x, y, false) {
    dir = pixel_thrdir(img, x, y, xp, yp);
    if (first) {
      prevdir = dir;
      len = 1;
      first = false;
    } else if (prevdir != dir) {
      /* We can't claim victory if this is the first sequence because it
         might wrap at the first point (stlen condition). */
      if ((minlen <= len) && (len <= maxlen) && (0 < stlen) &&
          (thrdir.SAME != prevdir)) then return true;
      if (0 == stlen) {
        stlen = len;
        stdir= prevdir;
      }
      len = 1;
      prevdir = dir;
    } else if (thrdir.SAME != dir) {
      len += 1;
    }
  }

  /* Check for wraps; if not, then this last piece or what we had at the
     start may also be good. */
  if (stdir == dir) {
    len += stlen;
    if ((minlen <= len) && (len <= maxlen) && (thrdir.SAME != dir)) then
      return true;
  } else if ((minlen <= len) && (len <= maxlen) && (thrdir.SAME != dir)) {
      return true;
  } else if ((minlen <= stlen) && (stlen <= maxlen) && (thrdir.SAME != stdir)) {
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
    returns:   thrdir classification at pt2
***/
inline proc pixel_thrdir(img : unmanaged clrimage?, x1 : int, y1 : int,
                         x2 : int, y2 : int) : thrdir {

  if (img!.c1(y2,x2) + thr <= img!.c1(y1,x1)) then return thrdir.LESS;
  else if (img!.c1(y1,x1) + thr <= img!.c1(y2,x2)) then return thrdir.MORE;
  else return thrdir.SAME;
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
  var retval : int;

  verify_setup();

  retval = PNG_read(inname.c_str(), rgb);
  end_onerr(retval, rgb);

  rgb_convert(rgb, clr, space);

  retval = mark_corners(clr, space, marked);
  end_onerr(retval, rgb, marked, clr);

  retval = PNG_write(outname.c_str(), marked, CLR_RGB);
  end_onerr(retval, rgb, marked, clr);

  free_rgbimage(rgb);
  free_rgbimage(marked);
  delete clr;

  return 0;
}
