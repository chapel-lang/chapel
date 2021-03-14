param n = 10;
param blk = 5;

var A1D = 1..n;
const A2D = {A1D,A1D}; 
var A: [A2D] real;

initA(A,'Adata.dat');
writeln(maxIndex(A));
writeln(maxIndex(abs(A)));

proc maxIndex(A:[?D]) {
  var maxA = A(D.low);
  var ind = D.low;

  for i in D {
    if A(i) > maxA {
       ind = i;
       maxA = A(i);
    }
  }
  return ind;
}
  
proc initA(A,filename:string){
  use IO;

  var Adat = open('Adata.dat', iomode.r).reader();

  for ij in A.domain {
    Adat.read(A(ij));
  }
  Adat.close();
}
