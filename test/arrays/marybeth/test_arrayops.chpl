param n = 10;
param blk = 5;

var A1D = 1..n;
const A2D = [A1D,A1D]; 
var A: [A2D] real;

initA(A,'Adata.dat');
writeln(maxIndex(A));
writeln(maxIndex(abs(A)));

def maxIndex(A:[?D]) {
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
  
def initA(A,filename:string){

  var Adat = file(filename,path='./',mode='r');

  Adat.open();
  for ij in A.domain {
    Adat.read(A(ij));
  }
  Adat.close();
}
