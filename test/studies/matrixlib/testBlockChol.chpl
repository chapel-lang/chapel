use IO;
use MatrixOps;

//Blocked Cholesky method.  Default test matrix is generated
//from the Matlab gallery - the lehmer matrix of order 10.

config const inputfile = "blockChol-Mat.dat";
config const upper = true;

proc main() {
  var Adat = open(inputfile, iomode.r).reader();

  const n = readSize(Adat);
  var blk = readBlk(Adat);
  var factor: string;

  // The blocksize cannot be less than 1.  Reset to 1 if this happens.
  // The blocksize cannot exceed the size of n.  Reset to n if this happens.
  blk = max(1,blk);
  blk = min(blk,n);

  var A1D = 1..n;
  const A2D = {A1D,A1D}; 
  var A: [A2D] real;
  initA(A,Adat);
  Adat.close();

  factor = if (upper) then "U" else "L";
  writeln("Unfactored Matrix:");
  writeln(A);
  writeln();

  blockChol(A,blk,factor);

  writeln("Factored Matrix:");
  writeCholFactor(A,factor);
  writeln();
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

proc writeCholFactor(A:[?D],fac:string) {
  var G:[D] A.eltType;

  if (fac == "U") {
    for i in D.dim(0) {
      for j in i..D.high(1){
        G(i,j) = A(i,j);
      }
    }
  }
  else {
    for j in D.dim(0) {
      for i in j..D.high(1) {
        G(i,j) = A(i,j);
      }
    }
  }
  writeln(G);
} 
