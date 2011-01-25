proc g(x: int, y: int) {
  writeln("In g(:int, :int)");
}

proc g(x: [] int, y) {
  writeln("In g(:[] int, :int)");
}

var A: [1..3] int;

g(A, 2);
