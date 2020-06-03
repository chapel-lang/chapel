// This program tests the blockLU routine in the MatrixOps module.
use MatrixOps;
use IO;

config const inputfile = "Amagic10.dat";

proc main() {
  var Adat = open(inputfile, iomode.r).reader();

  const n = readSize(Adat);
  var blk = readBlk(Adat);

  // The blocksize cannot be less than 1.  Reset to 1 if this happens.
  // The blocksize cannot exceed the size of n.  Reset to n if this happens.
  blk = max(1,blk);
  blk = min(blk,n);

  var A1D = 1..n;
  const A2D = {A1D,A1D}; 
  var A: [A2D] real;
  initA(A,Adat);
  Adat.close();

  var piv: [A1D] int;

  [i in A1D] piv(i) = i;

  writeln("Unfactored Matrix:");
  writeln(A);
  writeln();

  blockLU(A, blk, piv);

  writeln("Factored Matrix:");
  writeln(A);
  writeln();

  writeln("Pivot Vector:");
  writeln(piv);
}

proc readSize(Adat) {
  var n: int;

  Adat.read(n);
  return n;
} 

proc readBlk(Adat) {
  var blk: int;

  Adat.read(blk);
  return blk;
} 

proc initA(A,Adat){

  for ij in A.domain {
    Adat.read(A(ij));
  }
}
