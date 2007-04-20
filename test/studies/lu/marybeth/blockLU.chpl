// Block LU with pivoting.
// Magic square of dimension 10, is used as test matrix A.

// Using array alias operators to implement LU block algorithm
// with pivoting.
// In this version, A1D is a range, not a 1D domain.  The iterator,
// IterateByBlocks returns ranges, not subdomains of A1D.  Temporary
// ranges, slice1 and slice2 are used to define what should be
// subdomains sliced by indefinite ranges.  
//
// The maxIndex reduction routine and the swap operator are not
// implemented yet and need to be coded.  Since passing array aliases
// with slicing to functions doesn't seem to be fully supported yet,
// the pivot calculation and the swapping of pivot rows is coded inline
// in the algorithm.

def main() {
  config const inputfile = "Amagic10.dat";
  var Adat = file(inputfile,path='./',mode='r');
  Adat.open;

  const n = readSize(Adat);
  config var blk = readBlk(Adat);

  // The blocksize cannot exceed the size of n.  Reset to n if this happens.
  blk = min(blk,n);

  var A1D = 1..n;
  const A2D = [A1D,A1D]; 
  var A: [A2D] real;
  initA(A,Adat);
  Adat.close;

  var slice0, slice1, slice2: range;
  var piv: [A1D] int;
  var ind, temp:int;
  var rtemp: real;

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

def blockLU(x: [?D], piv, blk) where (D.rank != 2) {
  compilerError("blockLU factors a matrix.  The first
input parameter to blockLU must be a two-dimensional array.");
}

def blockLU(A: [?D], piv: [D.dim(1)], blk) where (D.rank == 2){
  var slice0, slice1, slice2: range;
  var ind: index(D);

  if ((D.dim(1).low != D.dim(2).low) | (D.dim(1).high != D.dim(2).high)) then 
    halt("error: blockLU requires square matrix with same dimensions");

  var A1D = D.dim(1);
  for (UnfactoredInds,CurrentBlockInds,TrailingBlockInds) 
    in IterateByBlocks(A1D,blk) {
  
    var A1 => A[UnfactoredInds,CurrentBlockInds];
    var A2 => A[UnfactoredInds,TrailingBlockInds];
    var A12 => A[CurrentBlockInds,TrailingBlockInds];
    var A22 => A[TrailingBlockInds,TrailingBlockInds];

// LU factorization of A1 
    for k in CurrentBlockInds {
//    temporaries used instead of subdomains with indefinite ranges.
      slice0 = k..UnfactoredInds.high;
      slice1 = k+1..UnfactoredInds.high;
      slice2 = k+1..CurrentBlockInds.high;

      ind = amaxIndex(A(slice0,k..k));

      if (ind(1) != k) {
        piv(k) <=> piv(ind(1));
        A(k..k,A1D) <=> A(ind(1)..ind(1),A1D);
      }

      if (A1(k,k) != 0.0) {
        forall i in slice1 {
          A1(i,k) = A1(i,k)/A1(k,k);
        }
        forall (i,j) in [slice1,slice2] {
          A1(i,j) -= A1(i,k)*A1(k,j);
        }
      } 
      else halt("zero pivot encountered");
    }

// Update of A12.
    forall j in TrailingBlockInds {
      for k in CurrentBlockInds {
//    temporary range used instead of subdomain with indefinite range
        slice2 = k+1..CurrentBlockInds.high;
        forall i in slice2 {
          A12(i,j) -= A1(i,k)*A12(k,j);
        }
      }
    }
// Update of A22 -= A12*A21.
    forall (i,j) in [TrailingBlockInds, TrailingBlockInds] {
      for k in CurrentBlockInds {
        A22(i,j) -= A1(i,k)*A12(k,j);
      }
    }
  }
}

iterator IterateByBlocks(D:range,blksize) {
  var n = D.length;
  var lo,hi: int;

  for i in D by blksize {
    lo = i;
    hi = min(i + blksize-1,n);
    yield (lo..n,i..hi,hi+1..n); 
  }   
}

def amaxIndex(A:[?D]) {
   var (_,ind) = maxloc reduce (abs(A),D);
   return ind;
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
