use BlockDist, CommDiagnostics, LayoutCS, LayoutCSUtil, Random,
    SparseBlockDistUtil;

enum layout { CSR, CSC };
use layout;

config const n = 10,
             density = 0.05,
             seed = 0,
             printSeed = seed == 0,
             skipDense = false,
             countComms = false;

var rands = if seed == 0 then new randomStream(real)
                         else new randomStream(real, seed);

// print library-selected seed, for reproducibility
if printSeed then
  writeln("Using seed: ", rands.seed);


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
  use List;
  var turnToken: atomic int;

  if countComms then startCommDiagnostics();
  coforall (locRow, locCol) in grid {
    on localeGrid[locRow, locCol] {
      var nnzs: list(2*int),
          vals: list(int);

      for srcloc in 0..<locsPerDim {
        // Make a local copy of the remote blocks of A and B; on my branch
        // this will also make a local copy of the remote indices, so long
        // as these are 'const'/read-only
        const Ablk = A.locArr[locRow, srcloc]!.myElems,
              Bblk = B.locArr[srcloc, locCol]!.myElems;
        
        local {
          for ac_br in Ablk.cols() {
            for (ar, a) in Ablk.rowsAndVals(ac_br) {
              for (bc, b) in Bblk.colsAndVals(ac_br) {
                nnzs.pushBack((ar,bc));
                vals.pushBack(a*b);
              }
            }
          }
        }
      }


      turnToken.waitFor(here.id);
      writeSparseMatrix("[" + here.id:string + "]'s local chunk of C:",
                        makeSparseMat(nnzs, vals));
      turnToken.write(here.id+1);
    }
  }
  if countComms {
    stopCommDiagnostics();
    printCommDiagnosticsTable();
  }
}


proc SparseMatMatMult(A: [?AD], B: [?BD]) {
  use List;

  var nnzs: list(2*int),
      vals: list(int);

  for ac_br in AD.cols() {
    for (ar, a) in A.rowsAndVals(ac_br) {
      for (bc, b) in B.colsAndVals(ac_br) {
        nnzs.pushBack((ar,bc));
        vals.pushBack(a*b);
      }
    }
  }

  return makeSparseMat(nnzs, vals);
}


proc DenseMatMatMult(A, B) {
  use List;

  var nnzs: list(2*int),
      vals: list(int);

  for i in 1..n {
    for k in 1..n {
      for j in 1..n {
        var prod = A[i,k] * B[k,j];
        if prod != 0 {
          nnzs.pushBack((i,j));
          vals.pushBack(prod);
        }
      }
    }
  }

  return makeSparseMat(nnzs, vals);
}


proc randSparseMatrix(Dom, density, param lay, locGrid) {
  type layoutType = CS(compressRows=(lay==CSR));
  const DenseBlockDom = Dom dmapped new blockDist(boundingBox=Dom,
                                                  targetLocales=locGrid,
                                                  sparseLayoutType=layoutType);
  var SD: sparse subdomain(DenseBlockDom);

  for (i,j) in Dom do
    if rands.next() <= density then
      SD += (i,j);

  return SD;
}


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




proc makeSparseMat(nnzs, vals) {
  var CDom: sparse subdomain(A.domain.parentDom);
  for ij in nnzs do
    CDom += ij;

  var C: [CDom] int;
  for (ij, c) in zip(nnzs, vals) do
    C[ij] += c;
  return C;
}
