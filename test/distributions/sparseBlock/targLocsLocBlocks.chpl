use BlockDist, CompressedSparseLayout;

config const n = 10,
             useTupleIndexing = true;

const D = {0..<n, 0..<n} dmapped new blockDist({0..<n, 0..<n},
                                               sparseLayoutType=cs(compressRows=true));
const DS: sparse subdomain(D);

// check that targetLocales on domains is working as expected
writeln("DS target locales:");
writeln(DS.targetLocales());
writeln();

var A: [DS] real;

// check that targetLocales on arrays is working as expected
writeln("A target locales:");
writeln(A.targetLocales());
writeln();

// build up the global sparse domain and array by having each locale set
// its own local blocks
coforall loc in DS.targetLocales() do on loc {
  // compute this locale's contribution to the index set, creating a
  // diagonal pattern per locale
  const myInds = DS.parentDom.localSubdomain();
  var locSpsInds: sparse subdomain(myInds) dmapped new cs(compressRows=true);

  for i in 0..<min(myInds.dim(0).size, myInds.dim(1).size) {
    locSpsInds += myInds.low + (i,i);
  }
  //  writeln(here.id, ": ", locSpsInds);

  // now do the same for its local non-zeroes
  var locVals: [locSpsInds] real;
  for i in 0..<min(myInds.dim(0).size, myInds.dim(1).size) {
    const low = myInds.low;
    locVals[low+(i,i)] = low(0)+i + (low(1)+i)/10.0;
  }
  //  writeln(here.id, ": ", locVals);

  // Take those local portions and plug them into the global domain/array
  DS.setLocalSubdomain(locSpsInds);
  A.setLocalSubarray(locVals);
}


// check the results

writeln("DS is:");
for i in D.dim(0) {
  for j in D.dim(1) {
    write(if DS.contains(i,j) then "X" else ".");
  }
  writeln();
}
writeln();


writeln("A is:");
writeSparseMatrix(A);

for locIdx in DS.targetLocales().domain {
  const remBlk = if useTupleIndexing then A.getLocalSubarray(locIdx)
                                     else A.getLocalSubarray((...locIdx));
  writeln(locIdx, " owns:");
  writeSparseMatrix(remBlk);
}


proc writeSparseMatrix(X) {
  const ref DnsDom = X.domain.parentDom;
  for i in DnsDom.dim(0) {
    for j in DnsDom.dim(1) {
      write(X[i,j], " ");
    }
    writeln();
  }
  writeln();
}
