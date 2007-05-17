// This file contains the function, blockLU, supporting 
// functions and iterators and a main function that tests
// the blockLU function.  
//
// The test matrix is read in from Adata.dat by default, 
// but a different input file can be specified on the
// command line.
//
// This blockLU function is written for a single locale.
// It uses an iterator to define blocks of indices that 
// make expressing the block algorithm easier and less 
// error-prone.  Array aliases are defined with these block 
// indices at each iteration, so that the submatrices involved 
// in the diagonal block factorization and subsequent updates 
// are easily referenced and modified.
// 

def blockLU(A: [?D], piv: [D.dim(1)], blk) where (D.rank == 2){
  // Block LU with pivoting.  This version is written for a single
  // locale and it assumes that the input matrix A is square and
  // has identical ranges for the two dimensions of the matrix.
  // The block size, blk, must be set to a valid block size - one
  // that is greater than zero and less than or equal to the dimension
  // of the matrix A.

  // Test that the domain of A is square with the same index set
  // for each dimension.
  if (D.dim(1) != D.dim(2)) then
    halt("blockLU requires square matrix with same dimensions");

  // Test that 0 < blk <= n, where n = length of one dimension of A.
  if (blk <= 0) | (blk > D.dim(1).length) then
    halt(blk," is an invalid block size passed to blockLU");

  var A1D = D.dim(1);
  var ind: index(D);
  const zero = 0.0:A.eltType;

  // Main loop of block LU uses an iterator to compute three
  // sets of block indices.
  for (UnfactoredInds,CurrentBlockInds,TrailingBlockInds) 
    in IterateByBlocks(A1D,blk) {
  
    // Assign array aliases for various submatrices of current
    // block iteration
    var A1 => A[UnfactoredInds,CurrentBlockInds];
    var A2 => A[UnfactoredInds,TrailingBlockInds];
    var A21 => A[TrailingBlockInds,CurrentBlockInds];
    var A12 => A[CurrentBlockInds,TrailingBlockInds];
    var A22 => A[TrailingBlockInds,TrailingBlockInds];

    // LU factorization of A1 
    for k in CurrentBlockInds {

      ind = amaxIndex(A(UnfactoredInds(k..),k..k));

      if (ind(1) != k) {
        piv(k) <=> piv(ind(1));
        A(k..k,..) <=> A(ind(1)..ind(1),..);
      }

      if (A1(k,k) != zero) {
        forall i in UnfactoredInds(k+1..) {
          A1(i,k) = A1(i,k)/A1(k,k);
        }
        forall (i,j) in [UnfactoredInds(k+1..),CurrentBlockInds(k+1..)] {
          A1(i,j) -= A1(i,k)*A1(k,j);
        }
      } 
      else halt("zero pivot encountered");
    }

    // Update of A12.
    forall j in TrailingBlockInds {
      for k in CurrentBlockInds {
        forall i in CurrentBlockInds(k+1..) {
          A12(i,j) -= A1(i,k)*A12(k,j);
        }
      }
    }
    // Update of A22 -= A21*A12.
    // MMIterator is used to generate the indices.  The loop
    // nesting order is not specified here, but in the iterator.
    // The iterator could also return tuple values for any of
    // indices, for loop unrolling, and the following code would
    // remain the same.
    for (i,j,k1,k2) in MMIterator(A21.domain,A12.domain) {
      A22(i,j) -= A21(i,k1)*A12(k2,j);
    }
  }
}

iterator IterateByBlocks(D:range,blksize) {
  // This iterator returns a tuple of ranges to be used
  // in each iteration of block LU.  This iterator takes
  // care of the end cases, so that testing for them is
  // not necessary in the block LU code itself.
  var end = D.high;
  var hi: int;

  for i in D by blksize {
    hi = min(i + blksize-1,end);
    yield (i..end,i..hi,hi+1..end); 
  }   
}
iterator MMIterator(D1, D2) {
  // An iterator to generate indices for a matrix multiplication
  // loop.
  for j in D2.dim(2) do
    for (k1,k2) in (D1.dim(2),D2.dim(1)) do
      for i in D1.dim(1) do
        yield (i,j,k1,k2);
}

def amaxIndex(A:[?D]) {
   var (_,ind) = maxloc reduce (abs(A),D);
   return ind;
}

def main() {
  config const inputfile = "Adata.dat";
  var Adat = file(inputfile,path='./',mode='r');
  Adat.open;

  const n = readSize(Adat);
  config var blk = readBlk(Adat);

  // The blocksize cannot be less than 1.  Reset to 1 if this happens.
  // The blocksize cannot exceed the size of n.  Reset to n if this happens.
  blk = max(1,blk);
  blk = min(blk,n);

  // Initialize A by reading in values from Adat.
  var A1D = 1..n;
  const A2D = [A1D,A1D]; 
  var A: [A2D] real;
  initA(A,Adat);
  Adat.close;

  var piv: [A1D] int;

  [i in A1D] piv(i) = i;

  writeln("Unfactored Matrix:");
  writeln(A);
  writeln();

  blockLU(A, piv, blk);

  writeln("Factored Matrix:");
  writeln(A);
  writeln();

  writeln("Pivot Vector:");
  writeln(piv);
}

def readSize(Adat) {
  var n: int;

  Adat.read(n);
  return n;
} 

def readBlk(Adat) {
  var blk: int;

  Adat.read(blk);
  return blk;
} 

def initA(A,Adat){

  for ij in A.domain {
    Adat.read(A(ij));
  }
}
