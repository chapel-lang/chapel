var a : (integer, integer) = (1, 2);
var b : (integer, integer) = (3, 4);
var c : (integer, integer);

writeln(a, ", ", b, ", ", c);

function foo(x : (integer, integer), y : (integer, integer)) {
  var t : (integer, integer);
  t(1) = x(1) + y(1);
  t(2) = x(2) + y(2);
  return t;
}

c = foo(a, b);

writeln(a, ", ", b, ", ", c);
