record A {
  var i : integer;
}
record B {
  var i : integer;
}

var a : A;
a.i = 1;
var b : B;
b.i = 1;

writeln("1 = ", a == b);
