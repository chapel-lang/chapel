use BlockDist, CompressedSparseLayout;

config const n = 10,
             useTupleIndexing = true;

config param csr = true;

const D = {0..<n, 0..<n} dmapped if csr then new blockDist({0..<n, 0..<n},
                                               sparseLayoutType=csrLayout)
                                        else new blockDist({0..<n, 0..<n});
const DS: sparse subdomain(D);

// check that targetLocales on domains is working as expected
writeln("DS target locales:");
writeln(DS.targetLocales());
writeln();

var A: [DS] real;

// build up the global sparse domain and array by having each locale set
// its own local blocks
coforall loc in DS.targetLocales() do on loc {
  // compute this locale's contribution to the index set, creating a
  // diagonal pattern per locale
  const myInds = DS.parentDom.localSubdomain();
  var locSpsInds: if csr then sparse subdomain(myInds) dmapped new csrLayout()
                         else sparse subdomain(myInds);

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


for block in A.localSubarrays() {
  writeln("locale ", here.id, " owns:\n", block);
}

forall block in A.localSubarrays() {
  forall a in block {
    a *= 10;
  }
}

/* Equivalent to the following loop, but already tested above
for block in A.localSubarrays() {
  writeln("locale ", here.id, " owns:\n", block);
}
*/

for loc in Locales {
  on loc {
    writeln("locale ", here.id, " owns:\n", A.getLocalSubarray());
  }
}
