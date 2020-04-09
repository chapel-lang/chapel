// Block LU with pivoting.
// Magic square of dimension 10 as test matrix.
use IO;

param n = 10;
param blk = 5;

const AD = {1..n,1..n};
const Vec = {1..n};
var A:[AD] real;
var piv: [Vec] int;
var colmax: real;
var temp: real;
var ind, itemp: int;

initA(A,'Adata.dat');
[i in Vec] piv(i) = i;

writeln("Unfactored Matrix:");
writeln(A);

for jblk in 1..n by blk {
  var low = jblk;
  var hi = if (jblk + blk-1) <= n then jblk+blk-1 else n;
  for k in low..hi {
    colmax = abs(A(k,k));
    ind = k;
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
    if (A(k,k) != 0.0) { 
      for i in k+1..n {
        A(i,k) = A(i,k)/A(k,k);
      }
    }
    for i in k+1..n {
      for j in k+1..hi {
        A(i,j) -= A(i,k)*A(k,j);
      }
    }
  }
  if (hi < n) {
    for j in 1..n-hi {
      for k in 1..blk {
        if (A(low + k-1,hi + j) != 0.0) {
          for i in k+1..blk {
            A(low+i-1,hi+j) = A(low+i-1,hi+j) - A(low+k-1,hi+j)*A(low+i-1,low+k-1);
          }
        }
      }
    }
    for i in hi+1..n {
      for j in hi+1..n {
        for k in low..hi {
          A(i,j) -= A(i,k)*A(k,j);
        }
      }
    }
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

