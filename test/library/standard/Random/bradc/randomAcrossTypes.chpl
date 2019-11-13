use Random;

config param useNPB = true;

var A: [1..8] real;
var B: [1..8] imag;
var C: [1..4] complex;

proc getRandomStream(seed: int, type eltType) {
  if useNPB then
    return NPBRandomStream.create(seed=seed, eltType=eltType);
  else
    return PCGRandomStream.create(seed=seed, eltType=eltType);
}

//
// Check standalone routines
//
fillRandom(A, seed=314159265, algorithm=rtype);
fillRandom(B, seed=314159265, algorithm=rtype);
fillRandom(C, seed=314159265, algorithm=rtype);
writeln(A);
writeln(B);
writeln(C);

var rs1 = getRandomStream(seed=314159265, eltType=real);
var rs2 = getRandomStream(seed=314159265, eltType=imag);
var rs3 = getRandomStream(seed=314159265, eltType=complex);

//
// Check fillRandom method
//
rs1.fillRandom(A);
rs2.fillRandom(B);
rs3.fillRandom(C);
writeln(A);
writeln(B);
writeln(C);

var rs4 = getRandomStream(seed=314159265, eltType=real);
var rs5 = getRandomStream(seed=314159265, eltType=imag);
var rs6 = getRandomStream(seed=314159265, eltType=complex);

//
// Check consecutive implementations of fillRandom method()
//
rs4.fillRandom(A[1..4]);
rs5.fillRandom(B[1..4]);
rs6.fillRandom(C[1..2]);
rs4.fillRandom(A[5..8]);
rs5.fillRandom(B[5..8]);
rs6.fillRandom(C[3..4]);
writeln(A);
writeln(B);
writeln(C);

//
// Check getNth() method
//
for i in 1..8 {
  A[i] = rs4.getNth(i);
  B[i] = rs5.getNth(i);
}
for i in 1..4 {
  C[i] = rs6.getNth(i);
}
writeln(A);
writeln(B);
writeln(C);

//
// Check skipToNth()/getNext() methods
//
for i in 1..8 {
  rs4.skipToNth(i);
  A[i] = rs4.getNext();
  rs5.skipToNth(i);
  B[i] = rs5.getNext();
}
for i in 1..4 {
  rs6.skipToNth(i);
  C[i] = rs6.getNext();
}
writeln(A);
writeln(B);
writeln(C);

var rs7 = getRandomStream(seed=314159265, eltType=real);
var rs8 = getRandomStream(seed=314159265, eltType=imag);
var rs9 = getRandomStream(seed=314159265, eltType=complex);
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
