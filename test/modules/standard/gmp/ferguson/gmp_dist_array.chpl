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
  x = i;
}
forall (x,i) in zip(B,D) {
  x = i;
}
forall (x,i) in zip(C,D) {
  x = i;
  x.fac((10000*i):uint(32));
}

forall (a,b,c) in zip(A,B,C) {
  c += a;
  c += b;
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


// Release the array elements.
// TODO: remove this section once the records are being released correctly.
// currently the array elements are leaking, so without this section we leak
// 4,064,968 bytes of GMP data. With this section we go back to leaking only 
// 2,460 bytes 
forall a in A {
  manual_free(a);
}
forall b in B {
  manual_free(b);
}
forall c in C {
  manual_free(c);
}

proc manual_free(ref bi : BigInt){
  if bi.owned then {
    // writeln("In the manual_free BigInt destructor and owned");
    on bi do mpz_clear(bi.mpz); 
    bi.owned = false;
  }
}
