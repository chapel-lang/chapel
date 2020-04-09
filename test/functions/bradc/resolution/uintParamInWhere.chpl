proc foo (x: uint(64), param y: uint(64)) where y == 0 {
  writeln("In uint foo");
}

proc foo(x,y) {
  writeln("In default foo");
}

enum color {
  red = 1,
  blue = 2
}

var u = 1: uint;

foo(u, color.red);
