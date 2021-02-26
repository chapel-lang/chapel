record Point {
  var x: real;
  var y: real;

  // This is a default initializer
  proc init() {
    writeln("Point default-init");
    this.x = 0.0;
    this.y = 0.0;
  }

  // This is a useful initializer for this type
  proc init(x: real, y: real) {
    writeln("initializing");
    this.x = x;
    this.y = y;
  }


  // This is a copy initializer
  proc init=(from: Point) {
    writeln("Point copy-init");
    this.x = from.x;
    this.y = from.y;
  }

}


proc = (ref lhs: Point, rhs: Point) {
  writeln("Point assignment");
  lhs.x = rhs.x;
  lhs.y = rhs.y;
}

{

  var a, b: Point;

  a = new Point(1.0, 2.0);
  b = new Point(4.0, 5.0);

  a; b;

}

