use BlockDist;
use GMP;

config const n = 20;
const Space = {1..n};
const D = Space dmapped Block(boundingBox=Space);

var A: [D] BigInt;
var B: [D] BigInt;
var C: [D] BigInt;

// Fill the arrays with something easy.
forall (x,i) in zip(A,D) {
  x = new BigInt(i);
  //x = i;
}
forall (x,i) in zip(B,D) {
  x = new BigInt(i);
  //x = i;
}
forall (x,i) in zip(C,D) {
  x = new BigInt(i);
  x.fac_ui((10000*i):uint(32));
  //x = i;
}

forall (a,b,c) in zip(A,B,C) {
  c.add(c,a);
  c.add(c,b);
  //c = a + b;
}
//writeln(C);

var sum = new BigInt(0);
for c in C {
  //writeln(c.locale, " ", c.sizeinbase(10));
  writeln(c.sizeinbase(10));
  sum.add(sum, c);
}
writeln(sum.sizeinbase(10));

var modulus = new BigInt("10000000000000000000000000000000000000000");
modulus.nextprime(modulus);
writeln(modulus);
sum.mod(sum, modulus);
writeln(sum);
delete modulus;
delete sum;

// Release the array elements.
forall a in A {
  delete a;
}
forall b in B {
  delete b;
}
forall c in C {
  delete c;
}
