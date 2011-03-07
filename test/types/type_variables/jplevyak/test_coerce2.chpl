proc foo(x1 : int, x2 : int) {
  writeln("foo w/ int 1 = ", x1, " and int 2 = ", x2);
}

proc foo(x1 : real, x2 : real) {
  writeln("foo w/ real 1 = ", x1, " and real 2 = ", x2);
}

proc foo(x1 : int, x2 : real) {
  writeln("foo w/ int 1 = ", x1, " and real 2 = ", x2);
}

var i = 2;
var f = 3.0;

foo(i, i);
foo(i, f);
foo(f, f);
