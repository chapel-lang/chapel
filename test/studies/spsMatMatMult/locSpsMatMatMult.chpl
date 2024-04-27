use LayoutCS, Random;

enum layout { csr, csc };

config const n = 10,
             density = 0.05,
             seed = 0;

var rands = if seed == 0 then new randomStream(real)
                         else new randomStream(real, seed);

// print library-selected seed, for reproducibility
if seed == 0 then
  writeln("Using seed: ", rands.seed);


const Dom = {1..n, 1..n};



const AD = randSparseMatrix(Dom, density, layout.csc),
      BD = randSparseMatrix(Dom, density, layout.csr);

//writeln("AD is:\n", AD);
//writeln("BD is:\n", BD);

var A: [AD] int = 1,
    B: [BD] int = 1;

writeSparseMatrix("A is:", A);
writeSparseMatrix("B is:", B);

SparseMatMatMult(A, B);
DenseMatMatMult(A, B);

proc SparseMatMatMult(A: [?AD], B: [?BD]) {
  use List;

  var nnzs: list(2*int),
      vals: list(int);

  for ac_br in AD.colRange {
    var bi = BD.startIdx[ac_br];
    for ai in AD.startIdx[ac_br]..<AD.startIdx[ac_br+1] {
      const ar = AD.idx[ai];
      writeln("Found A[", ar, ",", ac_br, "] = ", A.data[ai]);
      for bi in BD.startIdx[ac_br]..<BD.startIdx[ac_br+1] {
        const bc = BD.idx[bi];
        writeln("Got a hit at A[", ar, ",", ac_br, "] and B[", ac_br, ", ",
                bc, "]");
        nnzs.pushBack((ar,bc));
        vals.pushBack(A.data[ai]*B.data[bi]);
      }
    }
  }
  var CDom: sparse subdomain(A.domain.parentDom);
  for ij in nnzs do
    CDom += ij;

  var C: [CDom] int;
  for (ij, c) in zip(nnzs, vals) do
    C[ij] += c;

  writeSparseMatrix("C is:", C);
}

/* first, wrong draft
proc SparseMatMatMult(A: [?AD], B: [?BD]) {
  for ac_br in AD.colRange {
    var bi = BD.startIdx[ac_br];
    for ai in AD.startIdx[ac_br]..<AD.startIdx[ac_br+1] {
      const ar = AD.idx[ai];
      writeln("Found A[", ar, ",", ac_br, "] = ", A.data[ai]);
      var bc: int;
      do {
        if bi >= BD.startIdx[ac_br+1] then break;
        bc = BD.idx[bi];
        writeln("Checking B[", ac_br, ", ", bc, "]");
        bi += 1;
      } while bc < ar;
      if bc == ar then
        writeln("Got a hit at A[", ar, ",", ac_br, "] and B[", ac_br, ", ",
                bc, "]");
    }
  }
}
*/


// dense, expensive n**3 algorithm, just as a double-check

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

  var CDom: sparse subdomain(A.domain.parentDom);
  for ij in nnzs do
    CDom += ij;

  var C: [CDom] int;
  for (ij, c) in zip(nnzs, vals) do
    C[ij] += c;

  writeSparseMatrix("C is:", C);
}

proc randSparseMatrix(Dom, density, param lay) {
  var SD: sparse subdomain(Dom) dmapped CS(compressRows=(lay==layout.csr));

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
