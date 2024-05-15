use LayoutCS, LayoutCSUtil, Random;

enum layout { CSR, CSC };
use layout;

config const n = 10,
             density = 0.05,
             seed = 0,
             printSeed = seed == 0,
             skipDense = false;

var rands = if seed == 0 then new randomStream(real)
                         else new randomStream(real, seed);

// print library-selected seed, for reproducibility
//
if printSeed then
  writeln("Using seed: ", rands.seed);


// dense bounding box of the matrices
//
const Dom = {1..n, 1..n};

// sparse index patterns for the matrices
//
const AD = randSparseMatrix(Dom, density, CSC),
      BD = randSparseMatrix(Dom, density, CSR);

// the sparse matrices themselves
//
var A: [AD] int = 1,
    B: [BD] int = 1;

writeSparseMatrix("A is:", A);
writeSparseMatrix("B is:", B);


// compute and print the product using the sparse matrix-matrix algorithm
//
const Csps = SparseMatMatMult(A, B);
writeSparseMatrix("C (sparsely computed) is:", Csps);


// optionally compute and print the product using dense matrix-matrix
// multiplication as a check
//
if !skipDense {
  const Cdns = DenseMatMatMult(A, B);
  writeSparseMatrix("C (densely computed) is: ", Cdns);

  forall (i,j) in Cdns.domain do
    if Cdns[i,j] != Csps[i,j] then
      halt("Mismatch between dense and sparse algorithms at ", (i,j), ": ",
           Cdns[i,j], Csps[i,j]);
}


// sparse, outer, matrix-matrix multiplication algorithm; A is assumed
// CSC and B CSR
//
// TODO: parallelize algorithm
//
proc SparseMatMatMult(A, B) {
  use Map;

  var spsData: map(2*int, int);

  for ac_br in A.cols() {
    for (ar, a) in A.rowsAndVals(ac_br) {
      for (bc, b) in B.colsAndVals(ac_br) {
        const prod = a * b;
        if prod != 0 {
          if spsData.contains((ar,bc)) {
            spsData[(ar, bc)] += prod;
          } else {
            spsData.add((ar, bc), prod);
          }
        }
      }
    }
  }

  return makeSparseMat(Dom, spsData);
}


// dense, simple matrix-matrix multiplication algorithm; this is
// wildly inefficient, both because it ignores the sparsity and
// because it uses random access of the sparse arrays which tends to
// be expensive.
//
proc DenseMatMatMult(A, B) {
  use Map;

  var spsData: map(2*int, int);
  
  for i in 1..n {
    for j in 1..n {
      var prod = 0;

      for k in 1..n do
        prod += A[i,k] * B[k,j];

      if prod != 0 then
        spsData.add((i,j), prod);
    }
  }

  return makeSparseMat(Dom, spsData);
}


// create a random sparse matrix within the space of 'Dom' of the
// given density and layout
//
proc randSparseMatrix(Dom, density, param layout) {
  var SD: sparse subdomain(Dom) dmapped CS(compressRows=(layout==CSR));

  for (i,j) in Dom do
    if rands.next() <= density then
      SD += (i,j);

  return SD;
}


// print out a sparse matrix (in a dense format)
//
proc writeSparseMatrix(msg, Arr) {
  const ref SparseDom = Arr.domain,
            DenseDom = SparseDom.parentDom;

  writeln(msg);

  for r in DenseDom.dim(0) {
    for c in DenseDom.dim(1) {
      write(Arr[r,c], " ");
    }
    writeln();
  }
  writeln();    
}


// create a new sparse matrix from a map from sparse indices to values
//
proc makeSparseMat(parentDom, spsData) {
  use Sort;

  var CDom: sparse subdomain(parentDom) dmapped CS();
  var inds: [0..<spsData.size] 2*int;
  for (idx, i) in zip(spsData.keys(), 0..) do
    inds[i] = idx;

  sort(inds);
  
  for ij in inds do
    CDom += ij;

  var C: [CDom] int;
  for ij in inds do
    C[ij] += spsData[ij];

  return C;
}


// create a new sparse matrix from a collection of nonzero indices
// (nnzs) and values (vals)
//
proc makeSparseMat(parentDom, nnzs, vals) {
  var CDom: sparse subdomain(parentDom);
  for ij in nnzs do
    CDom += ij;

  var C: [CDom] int;
  for (ij, c) in zip(nnzs, vals) do
    C[ij] += c;
  return C;
}
