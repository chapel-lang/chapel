// Block LU without pivoting.
// Magic square of dimension 10, permuted according to
// Matlab's p matrix for LU (so it doesn't need pivoting),
// is used as test matrix A.

// This is a version of Block LU with array aliases that
// compiles and runs accurately.  There are a few modifications
// needed to make this run:  A1D is a range instead of a 1D domain,
// the output of the iterator IterateByBlocks is two ranges rather
// than two subdomains of A1D and the temporary range, slice, is 
// needed to represent the indefinite subdomain, CurrentBlockInds(k+1..). 
use IO;

param n = 10;
param blk = 5;

var A1D: range = 1..n;
const A2D = {A1D,A1D}; 
var A: [A2D] real;

// The variable slice is used to implement D(k+1..),
// where D would be a subdomain (but is currently a range).
var slice: range;

initA(A,'Adata.dat');

writeln("Unfactored Matrix:");
writeln(A);

for (CurrentBlockInds,TrailingBlockInds) in IterateByBlocks(A1D,blk) {
  
  ref A11 = A[CurrentBlockInds,CurrentBlockInds];
  ref A21 = A[TrailingBlockInds,CurrentBlockInds];
  ref A12 = A[CurrentBlockInds,TrailingBlockInds];
  ref A22 = A[TrailingBlockInds,TrailingBlockInds];
  
// LU factorization of A11 and A12.
  for k in CurrentBlockInds {

//  slice = CurrentBlockInds(k+1..)
    slice = k+1..CurrentBlockInds.high;
    if (A11(k,k) != 0.0) {
      forall i in slice {
        A11(i,k) = A11(i,k)/A11(k,k);
      }
      forall (i,j) in {slice,slice} {
        A11(i,j) -= A11(i,k)*A11(k,j);
      }
      forall i in TrailingBlockInds {
        A21(i,k) = A21(i,k)/A11(k,k);
      }
      forall (i,j) in {TrailingBlockInds, slice} {
        A21(i,j) -= A21(i,k)*A11(k,j);
      }
    }
    else halt("zero pivot encountered");
  }

// Update of A12.
  for j in TrailingBlockInds { // This loop needs to be serial
    for k in CurrentBlockInds {
//    slice = CurrentBlockInds(k+1..)
      slice = k+1..CurrentBlockInds.high; 
      // This loop can be parallel, but for the purposes of testing
      //  make it serial to get deterministic results.
      for i in slice {
        A12(i,j) -= A11(i,k)*A12(k,j);
      }
    }
  }
// Update of A22 -= A12*A21.
  forall (i,j) in {TrailingBlockInds, TrailingBlockInds} {
    for k in CurrentBlockInds {
      A22(i,j) -= A21(i,k)*A12(k,j);
    }
  }
}
writeln();
writeln("Factored Matrix:");
writeln(A);

iter IterateByBlocks(D:range,blksize) {
  var n = D.size;
  var lo,hi: int;

  for i in D by blksize {
    lo = i;
    hi = min(i+blksize-1,n);
    yield (i..hi,hi+1..n);
  }   
}
  
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


