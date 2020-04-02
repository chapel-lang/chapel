type indexType = int,
     elemType = real;

//
// Configuration constants indicating the problem size (n) and the
// block size (blkSize)
//
config const n = 5, blkSize = 3;

const tl1=1, tl2=1;

  const AbD: domain(2, indexType) = {1..n, 1..n+1};

  var Ab : [AbD] elemType,           // the matrix A and vector b
      piv: [1..n] indexType;         // a vector of pivot values

  var x: [1..n] int;

  const validAnswer = verifyResults(x);


proc backwardSubVerifyLocal(Ab, xToCheck, n: indexType) {
  writeln("backwardSubVerifyLocal() SKIPPED");
  return true;

  const bd = 1..n; // did not work: xToCheck.domain;  // or simply 1..n
//  assert(bd.dim(0) == Ab.domain.dim(0));
//  assert(bd == Ab.domain.dim(0), "bsVL-1");
  var x: [bd] elemType;

  for i in bd by -1 do
  {
    x[i] = (Ab[i,n+1] - (+ reduce [j in i+1..bd.high] (Ab[i,j] * x[j]))) 
            / Ab[i,i];
  }
  return true;
}


proc verifyResults(x) {
  ref ABglobal = Ab;
  var checkOK: bool;
 { // hide global Ab
  var Ab: [1..n, 1..n+1] elemType;
  if numLocales <= 4 {
    Ab = ABglobal;
  } else {
    forall (g,l) in zip(ABglobal,Ab) do l = g;
  }

  return backwardSubVerifyLocal(Ab, x, n);
 }  
}
