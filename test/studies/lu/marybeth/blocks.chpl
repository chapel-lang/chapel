// Block LU without pivoting.
// Magic square of dimension 10, permuted according to
// Matlab's p matrix for LU (so it doesn't need pivoting),
// is used as test matrix A.
use IO;

param n = 10;
param blk = 5;

const AD = {1..n,1..n};
var A: [AD] real;

initA(A,'Adata.dat');

writeln("Unfactored Matrix:");
writeln(A);

for jblk in 1..n by blk {
  var low = jblk;
  var hi = if (jblk + blk-1) <= n then jblk+blk-1 else n;
  var D11 = {low..hi,low..hi};
  var D21 = {hi+1..n,low..hi};
  var D12 = {low..hi,hi+1..n};
  var D22 = {hi+1..n,hi+1..n};
  for k in D11.dim(1) {
    if (A(k,k) != 0.0) {
      for i in k+1..hi {
        A(i,k) = A(i,k)/A(k,k);
      }
      for i in k+1..hi {
        for j in k+1..hi {
          A(i,j) -= A(i,k)*A(k,j);
        }
      }
    }
  }
  if (hi < n) {
    for k in D21.dim(1) {
      if A(k,k) != 0.0 {
        for i in D21.dim(0) {
          A(i,k) = A(i,k)/A(k,k);
        }
        for i in D21.dim(0) {
          for j in k+1..hi {
            A(i,j) -= A(i,k)*A(k,j);
          }
        }
      }
    }
    for j in D12.dim(1) {
      for k in low..hi {
        if (A(k,j)!= 0.0) {
          for i in k+1..hi {
            A(i,j) -= A(k,j)*A(i,k);
          }
        }
      }
    }
    for ind in D22 {
      var i = ind(0);
      var j = ind(1);
      for k in low..hi {
        A(i,j) -= A(i,k)*A(k,j);
      }
    }
  }
}
writeln();
writeln("Factored Matrix:");
writeln(A);

proc initA(A,filename:string){

// Create full permutation matrix to permute A.
// Very expensive, but easy way to permute the matrix
// so that pivoting isn't needed.

var Adat = open(filename,iomode.r).reader();
var P, temp: [A.domain] real;

for ij in A.domain {
  Adat.read(A(ij));
}
Adat.close();

P = 0.0;
P(1,2) = 1.0;
P(2,3) = 1.0;
P(3,7) = 1.0;
P(4,8) = 1.0;
P(5,9) = 1.0;
P(6,5) = 1.0;
P(7,1) = 1.0;
P(8,4) = 1.0;
P(9,10) = 1.0;
P(10,6) = 1.0;

temp = 0.0;
const i = 1..10;
for i in 1..10 {
  for j in 1..10 {
    for k in 1..10 {
      temp(i,j) = temp(i,j) + P(i,k)*A(k,j);
    }
  }
}
A = temp;
}
