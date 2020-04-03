/* Write a matrix to filename */
proc writeMatrix(fname, mat) where isCSArr(mat) {
  use IO;
  var f = open(fname, iomode.cw);
  var c = f.writer();
  var (m,n) = mat.shape;
  c.writeln(m);
  c.writeln(n);
  for (i,j) in mat.domain {
    c.writeln(mat[i,j],' ', i, ' ', j);
  }
  c.close();
}

/* Populate sparse matrix with random elements according to "nnz" */
proc populate(ref A, ref ADom, nnz: int, seed: int) where isCSArr(A) {
  use Random;
  //const nnz = (ADom._value.parentDom.size * sparsity): int;
  var indices: [1..nnz] 2*int;
  var randomIndices = createRandomStream(eltType=int, seed=seed);
  for idx in indices {
    // Ensure no duplicates
    var newIdx = idx;
    while indices.find(newIdx)(1) {
      newIdx = (randomIndices.getNext(ADom.dim(0).low, ADom.dim(0).high),
                randomIndices.getNext(ADom.dim(1).low, ADom.dim(1).high));
    }
    idx = newIdx;
  }

  ADom += indices;

  var randomReals = createRandomStream(eltType=real, seed=seed);
  for idx in ADom {
    A[idx] = 2.0; //randomReals.getNext();
  }
}

