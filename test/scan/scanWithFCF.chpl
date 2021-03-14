use BlockDist;

var A = newBlockArr({1..10}, real);
A = 1.0;
var B = + scan A;

proc foo() {
  writeln("In foo");
  return 42;
}

writeln("foo is: " + foo:string);
writeln(B);
