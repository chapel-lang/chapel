record A {
  var i : int;
}
record B {
  var i : int;
}

var a : A;
a.i = 1;
var b : B;
b.i = 2;

writeln("false = ", a == b);
