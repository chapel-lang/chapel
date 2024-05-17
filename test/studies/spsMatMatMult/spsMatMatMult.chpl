use CommDiagnostics, SparseBlockDistUtil, SpsMatUtil;

config const n = 10,
             density = 0.05,
             skipDense = false,
             countComms = false;

const locsPerDim = sqrt(numLocales:real): int,
      grid = {0..<locsPerDim, 0..<locsPerDim},
      localeGrid = reshape(Locales[0..<grid.size], grid);


if grid.size != numLocales then
  writeln("Warning: Only using ", grid.size, " of ", numLocales,
          " locales");
//writeln(grid);


const Dom = {1..n, 1..n};



const AD = randSparseMatrix(Dom, density, CSC, localeGrid),
      BD = randSparseMatrix(Dom, density, CSR, localeGrid);

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


proc SummaSparseMatMatMult(A: [?AD], B: [?BD]) {
  var CD = emptySparseDomLike(B);  // For now, hard-code C to use CSR, like B
  var C: [CD] int;

  ref targLocs = A.targetLocales();
  
  if countComms then startCommDiagnostics();

  coforall (locRow, locCol) in targLocs.domain {
    on targLocs[locRow, locCol] {
      var spsData: sparseMatDat;

      for srcloc in 0..<locsPerDim {
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
