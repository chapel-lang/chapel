use CommDiagnostics, SpsMatUtil;

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

SummaSparseMatMatMult(A, B);

/*
const CSps = SparseMatMatMult(A, B);

writeSparseMatrix("C (sparsely computed) is:", CSps);

if !skipDense {
  const CDns = DenseMatMatMult(A, B);
  writeSparseMatrix("C (densely computed) is: ", CDns);
}
*/


proc SummaSparseMatMatMult(A: [?AD], B: [?BD]) {
  var turnToken: atomic int;

  var (CD, C) = emptySparseDomLike(A);
  
  if countComms then startCommDiagnostics();

  coforall (locRow, locCol) in grid {
    on localeGrid[locRow, locCol] {
      var spsData: sparseMatDat;

      for srcloc in 0..<locsPerDim {
        // Make a local copy of the remote blocks of A and B; on my branch
        // this will also make a local copy of the remote indices, so long
        // as these are 'const'/read-only
        const aBlk = A.locArr[locRow, srcloc]!.myElems,
              bBlk = B.locArr[srcloc, locCol]!.myElems;

        local {
          sparseMatMatMult(aBlk, bBlk, spsData);
        }
      }

      const myInds = A.domain.parentDom.localSubdomain();
      var cBlk = makeSparseMat(myInds, spsData);
      CD.myLocDom!.mySparseBlock = cBlk.domain;
      //      C.myLocArr!.myElems._value.data = cBlk.data;

      turnToken.waitFor(here.id);
      writeSparseMatrix("[" + here.id:string + "]'s local chunk of C["
                        + myInds:string + "]:", cBlk);
      turnToken.write(here.id+1);
    }
  }

  for loc in grid do
    writeln("CD[",loc,"] is: ", CD.locDoms[loc]);
  writeSparseMatrix("C is: ", C);

  if countComms {
    stopCommDiagnostics();
    printCommDiagnosticsTable();
  }
}
