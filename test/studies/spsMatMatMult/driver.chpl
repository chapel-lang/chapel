use ChplConfig, MatMatMult, SpsMatUtil;

config param distributed = !compiledForSingleLocale();

config const n = 10,
             density = 0.05,
             printMatrices = true,
             skipDense = false;

// dense bounding box of the matrices
//
const Dom = {1..n, 1..n};

// compute the sparse, random index patterns for the matrices
//
const AD = randSparseMatrix(Dom, density, CSC, distributed),
      BD = randSparseMatrix(Dom, density, CSR, distributed);

// the sparse matrices themselves
//
var A: [AD] int = 1,
    B: [BD] int = 1;

if printMatrices {
  writeSparseMatrix("A is:", A);
  writeSparseMatrix("B is:", B);
}

// compute and print the product using the sparse matrix-matrix algorithm
//
const CSps = if distributed then SummaSparseMatMatMult(A, B)
                            else sparseMatMatMult(A, B);

if printMatrices then
  writeSparseMatrix("C (sparsely computed) is:", CSps);

// optionally compute and print the product using dense matrix-matrix
// multiplication as a check
//
if !skipDense {
  const CDns = denseMatMatMult(A, B);

  if printMatrices then
    writeSparseMatrix("C (densely computed) is: ", CDns);

  forall (i,j) in CDns.domain do
    if CDns[i,j] != CSps[i,j] then
      halt("Mismatch between dense and sparse algorithms at ", (i,j), ": ",
           CDns[i,j], CSps[i,j]);
}
