var a : (int, int) = (1, 2);
var b : (int, int) = (3, 4);
var c : (int, int);

writeln(a, ", ", b, ", ", c);

proc foo(x : (int, int), y : (int, int)) {
  var t : (int, int);
  t(0) = x(0) + y(0);
  t(1) = x(1) + y(1);
  return t;
}

c = foo(a, b);

writeln(a, ", ", b, ", ", c);
