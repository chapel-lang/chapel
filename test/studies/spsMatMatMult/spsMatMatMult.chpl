use CommDiagnostics, SparseBlockDistUtil, SpsMatUtil;

config const n = 10,
             density = 0.05,
             skipDense = false,
             countComms = false;

const Dom = {1..n, 1..n};

const AD = randSparseMatrix(Dom, density, CSC, distributed=true),
      BD = randSparseMatrix(Dom, density, CSR, distributed=true);

var A: [AD] int = 1,
    B: [BD] int = 1;

writeSparseMatrix("A is:", A);
writeSparseMatrix("B is:", B);

const CSps = SummaSparseMatMatMult(A, B);

writeSparseMatrix("C (sparsely computed) is:", CSps);

if !skipDense {
  const CDns = denseMatMatMult(A, B);
  writeSparseMatrix("C (densely computed) is: ", CDns);

  forall (i,j) in CDns.domain do
    if CDns[i,j] != CSps[i,j] then
      halt("Mismatch between dense and sparse algorithms at ", (i,j), ": ",
           CDns[i,j], CSps[i,j]);
}
