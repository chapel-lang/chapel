use BlockDist;
use BigInteger;

config const n     = 20;
const        Space = { 1 .. n };
const        D     = Space dmapped new blockDist(boundingBox = Space);

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
  fac(x, (10000 * i) : uint(32));
}

forall (a,b,c) in zip(A,B,C) {
  add(c, c, a);
  add(c, c, b);
}

var sum = new bigint(0);

for c in C {
  writeln(c.sizeInBase(10));

  add(sum, sum, c);
}

writeln(sum.sizeInBase(10));

var modulus = new bigint("10000000000000000000000000000000000000000");

nextPrime(modulus, modulus);

writeln(modulus);

mod(sum, sum, modulus);

writeln(sum);

