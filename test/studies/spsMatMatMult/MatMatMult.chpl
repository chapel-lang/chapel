module MatMatMult {
  use CommDiagnostics, Time;
  use SpsMatUtil;
  
  config const countComms=false,
               printTimings=false;

  // sparse, outer, matrix-matrix multiplication algorithm; A is assumed
  // CSC and B CSR
  //
  proc sparseMatMatMult(A, B) {
    if countComms then startCommDiagnostics();
    var time: stopwatch;
    time.start();

    var spsData: sparseMatDat;

    sparseMatMatMult(A, B, spsData);

    var C = makeSparseMat(A.domain.parentDom, spsData);
    
    const elapsed = time.elapsed();

    if countComms {
      stopCommDiagnostics();
      printCommDiagnosticsTable();
      writeln();
    }

    if printTimings then writeln("Elapsed time = ", elapsed, "\n");

    return C;
  }

  // This version forms the guts of the above and permits a running set
  // of nonzeroes to be passed in and updated rather than assuming that
  // the multiplication is the first/only step.
  //
  proc sparseMatMatMult(A, B, ref spsData) {
    forall ac_br in A.cols() with (merge reduce spsData) do
      for (ar, a) in A.rowsAndVals(ac_br) do
        for (bc, b) in B.colsAndVals(ac_br) do
          spsData.add((ar, bc), a * b);
  }

  proc sparseMatMatMult(A, B) where (!A.chpl_isNonDistributedArray() &&
                                     !B.chpl_isNonDistributedArray()) {
    var CD = emptySparseDomLike(B);  // For now, hard-code C to use CSR, like B
    var C: [CD] int;

    ref targLocs = A.targetLocales();
  
    if countComms then startCommDiagnostics();
    var time: stopwatch;
    time.start();

    coforall (locRow, locCol) in targLocs.domain {
      on targLocs[locRow, locCol] {
        var spsData: sparseMatDat;

        for srcloc in targLocs.dim(0) {
          // Make a local copy of the remote blocks of A and B; on my branch
          // this will also make a local copy of the remote indices, so long
          // as these are 'const'/read-only
          //
          const aBlk = A.getLocalSubarray(locRow, srcloc),
                bBlk = B.getLocalSubarray(srcloc, locCol);

          // This local block is not strictly necessary but ensures that the
          // computation on the blocks will not require communication
          local {
            sparseMatMatMult(aBlk, bBlk, spsData);
          }
        }

        // Get my locale's local indices and create a sparse matrix
        // using them and the spsData computed above.
        //
        const locInds = A.domain.parentDom.localSubdomain();
        var cBlk = makeSparseMat(locInds, spsData);

        // Stitch the local portions back together into the global-view
        //
        CD.setLocalSubdomain(cBlk.domain);
        C.setLocalSubarray(cBlk);
      }
    }

    const elapsed = time.elapsed();
    
    if countComms {
      stopCommDiagnostics();
      printCommDiagnosticsTable();
      writeln();
    }

    if printTimings then writeln("Elapsed time = ", elapsed, "\n");

    return C;
  }


  // dense, simple matrix-matrix multiplication algorithm; this is
  // wildly inefficient, both because it ignores the sparsity and
  // because it uses random access of the sparse arrays which tends to
  // be expensive.
  //
  proc denseMatMatMult(A, B) {
    if countComms then startCommDiagnostics();
    var time: stopwatch;
    time.start();

    const n = A.dim(0).size;
    
    var spsData: sparseMatDat;
  
    for i in 1..n {
      for j in 1..n {
        var prod = 0;

        forall k in 1..n with (+ reduce prod) do
          prod += A[i,k] * B[k,j];

        if prod != 0 then
          spsData.add((i,j), prod);
      }
    }

    var C = makeSparseMat(A.domain.parentDom, spsData);

    const elapsed = time.elapsed();

    if countComms {
      stopCommDiagnostics();
      printCommDiagnosticsTable();
      writeln();
    }

    if printTimings then writeln("Elapsed time = ", elapsed, "\n");

    return C;
  }
}
