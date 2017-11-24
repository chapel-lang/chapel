use BlockDist;
use BigInteger;

config const n     = 20;
const        Space = { 1 .. n };
const        D     = Space dmapped Block(boundingBox = Space);

var A: [D] bigint;
var B: [D] bigint;
var C: [D] bigint;

// Fill the arrays with something easy.
forall (x,i) in zip(A,D) {
  x = new bigint(i);
}

forall (x,i) in zip(B,D) {
  x = new bigint(i);
}

forall (x,i) in zip(C,D) {
  x = new bigint(i);
  x.fac((10000 * i) : uint(32));
}

forall (a,b,c) in zip(A,B,C) {
  c.add(c,a);
  c.add(c,b);
}

var sum = new bigint(0);

for c in C {
  writeln(c.sizeinbase(10));

  sum.add(sum, c);
}

writeln(sum.sizeinbase(10));

var modulus = new bigint("10000000000000000000000000000000000000000");

modulus.nextprime(modulus);

writeln(modulus);

sum.mod(sum, modulus);

writeln(sum);

