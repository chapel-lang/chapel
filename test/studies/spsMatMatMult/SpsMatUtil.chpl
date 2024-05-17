module SpsMatUtil {
  use BlockDist, LayoutCS, LayoutCSUtil, Map, Random;

  enum layout { CSR, CSC };
  public use layout;

  config const seed = 0,
               printSeed = seed == 0;

  var rands = if seed == 0 then new randomStream(real)
                         else new randomStream(real, seed);

  // print library-selected seed, for reproducibility
  //
  if printSeed then
    writeln("Using seed: ", rands.seed);

  record sparseMatDat {
    forwarding var m: map(2*int, int);

    proc ref add(idx: 2*int, val: int) {
      if val != 0 {
        if m.contains(idx) {
          m[idx] += val;
        } else {
          m.add(idx, val);
        }
      }
    }
  }

  // sparse, outer, matrix-matrix multiplication algorithm; A is assumed
  // CSC and B CSR
  //
  proc sparseMatMatMult(A, B) {
    var spsData: sparseMatDat;

    sparseMatMatMult(A, B, spsData);

    return makeSparseMat(A.domain.parentDom, spsData);
  }

  // This version forms the guts of the above and permits a running set
  // of nonzeroes to be passed in and updated rather than assuming that
  // the multiplication is the first/only step.
  //
  proc sparseMatMatMult(A, B, ref spsDataMap) {
    forall ac_br in A.cols() with (merge reduce spsDataMap) do
      for (ar, a) in A.rowsAndVals(ac_br) do
        for (bc, b) in B.colsAndVals(ac_br) do
          spsDataMap.add((ar, bc), a * b);
  }


  // dense, simple matrix-matrix multiplication algorithm; this is
  // wildly inefficient, both because it ignores the sparsity and
  // because it uses random access of the sparse arrays which tends to
  // be expensive.
  //
  proc denseMatMatMult(A, B) {
    const n = A.dim(0).size;
    
    var spsData: sparseMatDat;
  
    for i in 1..n {
      for j in 1..n {
        var prod = 0;

        for k in 1..n do
          prod += A[i,k] * B[k,j];

        if prod != 0 then
          spsData.add((i,j), prod);
      }
    }

    return makeSparseMat(A.domain.parentDom, spsData);
  }


  // create a local random sparse matrix within the space of 'Dom' of
  // the given density and layout
  //
  proc randSparseMatrix(Dom, density, param layout) {
    var SD: sparse subdomain(Dom) dmapped CS(compressRows=(layout==CSR));

    for (i,j) in Dom do
      if rands.next() <= density then
        SD += (i,j);

    return SD;
  }


  // create a block-distributed random sparse matrix within the space
  // of 'Dom' of the given density and layout
  //
  proc randSparseMatrix(Dom, density, param layout, targetLocales) {
    type layoutType = CS(compressRows=(layout==CSR));
    const DenseBlkDom = Dom dmapped new blockDist(boundingBox=Dom,
                                                  targetLocales=targetLocales,
                                                  sparseLayoutType=layoutType);

    var SD: sparse subdomain(DenseBlkDom);

    for (i,j) in Dom do
      if rands.next() <= density then
        SD += (i,j);

    return SD;
  }


  proc emptySparseDomLike(Mat) {
    var ResDom: sparse subdomain(Mat.domain.parentDom);
    return ResDom;
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
      try! C[ij] += spsData[ij];  // TODO: Should this really throw?

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


  // This is a custom reduction, and a good case study for why our custom
  // reduction interface needs a refresh
  //
  class merge: ReduceScanOp {
    type eltType = sparseMatDat;
    var value: eltType;  // TODO: lots of deep copying here to avoid

    proc identity {
      var ident: eltType;
      return ident;
    }

    /*
    proc initialAccumulate(x) {
      if x.size != 0 then
        halt("Error shouldn't call merge.initialAccumulate() with a non-empty table");
    }
    */

    proc accumulate(x) {
      // Why is this ever called with a sparseMatDat as the argument?!?
      for (k,v) in zip(x.keys(), x.values()) {
        if value.contains(k) {
          value[k] += v;
        } else {
          value.add(k, v);
        }
      }
    }
    
    proc accumulateOntoState(ref state, x) {
      halt("Error, shouldn't call merge.accumulateOntoState()");
    }

    proc combine(x) {
      accumulate(x.value);
    }

    proc generate() {
      return value;
    }
    
    inline proc clone() {
      return new unmanaged merge(eltType=eltType);
    }
  }
}
