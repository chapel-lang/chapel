record A {
  var i : integer;
}
record B {
  var i : integer;
}

var a : A;
a.i = 1;
var b : B;
b.i = 2;

writeln("0 = ", a == b);
