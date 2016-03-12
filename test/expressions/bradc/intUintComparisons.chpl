var i1: int = -1;
var i2: int = 3;
var u: uint = 2;

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
