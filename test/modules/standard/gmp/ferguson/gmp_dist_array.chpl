use BlockDist;
use BigInteger;

config const n     = 20;
const        Space = { 1 .. n };
const        D     = Space dmapped Block(boundingBox = Space);

var A: [D] Bigint;
var B: [D] Bigint;
var C: [D] Bigint;

// Fill the arrays with something easy.
forall (x,i) in zip(A,D) {
  x = new Bigint(i);
}

forall (x,i) in zip(B,D) {
  x = new Bigint(i);
}

forall (x,i) in zip(C,D) {
  x = new Bigint(i);
  x.fac((10000 * i) : uint(32));
}

forall (a,b,c) in zip(A,B,C) {
  c.add(c,a);
  c.add(c,b);
}

var sum = new Bigint(0);

for c in C {
  writeln(c.sizeinbase(10));

  sum.add(sum, c);
}

writeln(sum.sizeinbase(10));

var modulus = new Bigint("10000000000000000000000000000000000000000");

modulus.nextprime(modulus);

writeln(modulus);

sum.mod(sum, modulus);

writeln(sum);

