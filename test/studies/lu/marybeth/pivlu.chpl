//  Simple LU algorithm with pivoting.
//  Using 1D slices into A for i only.
//  Magic square of dimension 10 is used as test matrix.
use IO;

param n = 10;

const AD = {1..n,1..n};
const Vec = {1..n};
var A: [AD] real;
var piv: [Vec] int;
var colmax: real;
var temp: real;
var ind, itemp: int;

initA(A,'Adata.dat');
[i in Vec] piv(i) = i;

writeln("Unfactored Matrix:");
writeln(A);

for k in 1..n-1 {
  colmax = abs(A(k,k));
  for i in k+1..n {
    if (abs(A(i,k)) > colmax) {
      colmax = abs(A(i,k));
      ind = i;
    }
  }
  if (ind != k) {
    itemp = piv(k);
    piv(k) = piv(ind);
    piv(ind) = itemp;
    for i in 1..n {
      temp = A(k,i);
      A(k,i) = A(ind,i);
      A(ind,i) = temp;
    }
  }
  const i = k+1..n;
  if (A(k,k) != 0.0) { 
    A(i,k) = A(i,k)/A(k,k);
  }
  for j in k+1..n {
    A(i,j) -= A(i,k)*A(k,j);
  }
}
writeln();
writeln("Factored Matrix:");
writeln(A);
writeln();
writeln("Pivot Vector:");
writeln(piv);

proc initA(A,filename:string){

  var Adat = open(filename, iomode.r).reader();

  for ij in A.domain {
    Adat.read(A(ij));
  }
  Adat.close();
}

