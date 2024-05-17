module MatMatMult {
  use CommDiagnostics, SpsMatUtil;
  
  config const countComms=false;

  proc SummaSparseMatMatMult(A: [?AD], B: [?BD]) {
    var CD = emptySparseDomLike(B);  // For now, hard-code C to use CSR, like B
    var C: [CD] int;

    ref targLocs = A.targetLocales();
  
    if countComms then startCommDiagnostics();

    coforall (locRow, locCol) in targLocs.domain {
      on targLocs[locRow, locCol] {
        var spsData: sparseMatDat;

        for srcloc in targLocs.dim(0) {
          // Make a local copy of the remote blocks of A and B; on my branch
          // this will also make a local copy of the remote indices, so long
          // as these are 'const'/read-only
          //
          const aBlk = A.locArr[locRow, srcloc]!.myElems,
                bBlk = B.locArr[srcloc, locCol]!.myElems;

          // This local block is not strictly necessary but ensures that the
          // computation on the blocks will not require communication
          local {
            sparseMatMatMult(aBlk, bBlk, spsData);
          }
        }

        const locInds = A.domain.parentDom.localSubdomain();
        var cBlk = makeSparseMat(locInds, spsData);

        // Stitch the local portions back together into the global-view
        //
        // TODO: Create nicer interfaces for this
        //
        CD.myLocDom!.mySparseBlock = cBlk.domain;
        C.myLocArr!.myElems._value.data = cBlk.data;
      }
    }

    if countComms {
      stopCommDiagnostics();
      printCommDiagnosticsTable();
    }

    return C;
  }
}
