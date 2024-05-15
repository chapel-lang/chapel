use LayoutCSUtil, SpsMatMatUtil;

config const n = 10,
             density = 0.05,
             skipDense = false;

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
const Csps = sparseMatMatMult(A, B);
writeSparseMatrix("C (sparsely computed) is:", Csps);


// optionally compute and print the product using dense matrix-matrix
// multiplication as a check
//
if !skipDense {
  const Cdns = denseMatMatMult(A, B);
  writeSparseMatrix("C (densely computed) is: ", Cdns);

  forall (i,j) in Cdns.domain do
    if Cdns[i,j] != Csps[i,j] then
      halt("Mismatch between dense and sparse algorithms at ", (i,j), ": ",
           Cdns[i,j], Csps[i,j]);
}


