config const n = 4;
config const epsilon = 1.0e-5;

const D = [1..n,1..n] dmapped(block);

var A, Temp : [D.expand(1)] real;

A(D.exterior(1,0)) = 1.0;

var delta = 0.0;

do {
  [(i,j) in D] Temp(i,j) = (A(i-1,j) + A(i+1,j) + A(i,j-1) + A(i,j+1)) / 4.0;
  // also:
  //   Temp(D) = (A(D.translate(-1,0)) + A(D.translate(1,0)) + 
  //              A(D.translate(0,-1)) + A(D.translate(0,1))) / 4.0;
  delta = max reduce abs(A(D) - Temp(D));
  A(D) = Temp(D);
} while delta > epsilon;

writeln(A);
