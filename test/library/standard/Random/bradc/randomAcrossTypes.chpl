use Random;

var A: [1..8] real;
var B: [1..8] imag;
var C: [1..4] complex;

//
// Check standalone routines
//
fillRandom(A, seed=314159265);
fillRandom(B, seed=314159265);
fillRandom(C, seed=314159265);
writeln(A);
writeln(B);
writeln(C);

var rs1 = new randomStream(seed=314159265, eltType=real);
var rs2 = new randomStream(seed=314159265, eltType=imag);
var rs3 = new randomStream(seed=314159265, eltType=complex);

//
// Check fill method
//
rs1.fill(A);
rs2.fill(B);
rs3.fill(C);
writeln(A);
writeln(B);
writeln(C);

var rs4 = new randomStream(seed=314159265, eltType=real);
var rs5 = new randomStream(seed=314159265, eltType=imag);
var rs6 = new randomStream(seed=314159265, eltType=complex);

//
// Check consecutive implementations of fillRandom method()
//
rs4.fill(A[1..4]);
rs5.fill(B[1..4]);
rs6.fill(C[1..2]);
rs4.fill(A[5..8]);
rs5.fill(B[5..8]);
rs6.fill(C[3..4]);
writeln(A);
writeln(B);
writeln(C);

//
// Check getNth() method
//
for i in 1..8 {
  A[i] = rs4.getNth(i-1);
  B[i] = rs5.getNth(i-1);
}
for i in 1..4 {
  C[i] = rs6.getNth(i-1);
}
writeln(A);
writeln(B);
writeln(C);

//
// Check skipToNth()/getNext() methods
//
for i in 1..8 {
  rs4.skipToNth(i-1);
  A[i] = rs4.getNext();
  rs5.skipToNth(i-1);
  B[i] = rs5.getNext();
}
for i in 1..4 {
  rs6.skipToNth(i-1);
  C[i] = rs6.getNext();
}
writeln(A);
writeln(B);
writeln(C);

var rs7 = new randomStream(seed=314159265, eltType=real);
var rs8 = new randomStream(seed=314159265, eltType=imag);
var rs9 = new randomStream(seed=314159265, eltType=complex);
//
// Check getNext() method
//
for i in 1..8 {
  A[i] = rs7.getNext();
  B[i] = rs8.getNext();
}
for i in 1..4 {
  C[i] = rs9.getNext();
}
writeln(A);
writeln(B);
writeln(C);
