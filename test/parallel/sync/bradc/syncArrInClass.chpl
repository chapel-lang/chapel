class C {
  var arr: [1..3] sync int;
  proc init() {}
}

var myC = new unmanaged C();

writeln("Made it past constructor");

for i in 1..3 {
  writeln("myC.arr(", i, ") = ", myC.arr(i).readXX());
  writeln("myC.arr(", i, ").isFull = ", myC.arr(i).isFull);
}

delete myC;
