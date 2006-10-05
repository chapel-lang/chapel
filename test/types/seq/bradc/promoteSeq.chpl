var s: seq(int) = (/ 0, 1, 2, 3, 4 /);
var D: domain(1) = [-2..2];

var b = (s == 1);

writeln("b is: ", b);

var b2 = (D == 0);

writeln("b2 is: ", b2);

foo(s);
foo(D);

def foo(x) {
  var b3 = (x == 0);
  writeln("b3 is: ", b3);
}