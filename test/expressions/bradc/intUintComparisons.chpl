var i1: int = -1;
var i2: int = 3;
var u: uint = 2;
var u2: uint = 3;

proc testit(x, y) {
  writeln(x, " == ", y, " == ", x == y);
  writeln(x, " != ", y, " == ", x != y);
  writeln(x, " <  ", y, " == ", x <  y);
  writeln(x, " <= ", y, " == ", x <= y);
  writeln(x, " >  ", y, " == ", x >  y);
  writeln(x, " >= ", y, " == ", x >= y);
}

testit(i1, u);
testit(i2, u);
testit(u, i1);
testit(u, i2);
testit(i2, u2);
