class C {
  var arr: [1..3] sync int;
}

var myC = new unmanaged C();

writeln("Made it past constructor");

for i in 1..3 {
  writeln("myC.arr(", i, ") = ", myC.arr(i).readXX());
  writeln("myC.arr(", i, ").isFull = ", myC.arr(i).isFull);
}

delete myC;
