class C {
  var arr: [1..3] sync int;
}

var myC = C();

writeln("Made it past constructor");

for i in 1..3 do
  writeln("myC.arr(", i, ") = ", myC.arr(i).readXX());

