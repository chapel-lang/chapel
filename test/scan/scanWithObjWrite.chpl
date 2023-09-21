use BlockDist;

var A = blockDist.createArray({1..10}, real);
A = 1.0;
var B = + scan A;

proc foo() {
  writeln("In foo");
  return 42;
}

var o: RootClass = new RootClass();
writeln("o is: ", o);
writeln("B is: ", B);
