record Point {
  var x: real;
  var y: real;
}

proc =(ref lhs: 2*real, rhs: Point) {
  lhs[0] = rhs.x;
  lhs[1] = rhs.y;
}

var p = new Point(10,20);
var tup = (1.0, 2.0);
tup = p;
