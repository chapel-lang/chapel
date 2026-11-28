
/*****
      ex_class.chpl -
      Examples showing the use of this and these for a class.  See text for
      details.

      Call:
        ex_class

      c 2015-2018 Primordial Machine Vision Systems
*****/


class citer {
  var center_x, center_y : int;
  var closed_circle : bool;
  var r : int;

  /* Initializer only setting up the radius. */
  proc init(radius : int) {
    if (this.r != radius) {
      writeln("init r to ", radius);
      this.r = radius;
    }
  }

  /* Initializer specifying the complete circle. */
  proc init(radius : int, xc : int, yc : int, closed = true) {
    center_x = xc;
    center_y = yc;
    closed_circle = closed;
    if (this.r != radius) {
      writeln("init r to ", radius, " along with xc, yc, closed");
      this.r = radius;
    }
  }

  /* Instance access to change the radius. */
  proc set_radius(radius : int) {
    if (this.r != radius) {
      writeln("update r to ", radius);
      this.r = radius;
    }
  }

  /* Instance access that generates the circumference about a point. */
  proc this(xc : int, yc : int, closed = true) {
    center_x = xc;
    center_y = yc;
    closed_circle = closed;

    return these();
  }

  /* The iterator around the circle. */
  iter these() {
    for i in 0..r do
      yield (i + center_x, i + center_y);
  }
}

/* How To Use - 1. Create new instance and iterate over it.  for will call
   these to get sequence. */
writeln("Iterate over circle radius 5 at 1,1");
for i in new citer(5, 1, 1, false) do writeln("  iter ", i);

/* How To Use - 2. Instantiate an instance with the radius and use the 'this'
   method to shift the center/close the circle and call the iterator.  'this'
   is overloaded so that you can change only the radius. */
writeln("Iterate over circle radius 4 at 1,1");
var cinst = new unmanaged citer(4);
for i in cinst(1, 1) do writeln("  iter ", i);
writeln("Iterate over circle radius 5 at 3,2");
cinst.set_radius(5);
for i in cinst(3, 2) do writeln("  iter ", i);

delete cinst;
