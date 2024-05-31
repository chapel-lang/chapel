use BlockDist, LayoutCS;

config const n = 10;

const D = {0..<n, 0..<n} dmapped new blockDist({0..<n, 0..<n},
                                               sparseLayoutType=CS(compressRows=true));
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
  const myInds = DS.parentDom.localSubdomain();
  var locSpsInds: sparse subdomain(myInds) dmapped new dmap(new CS(compressRows=true));

  for i in 0..<min(myInds.dim(0).size, myInds.dim(1).size) {
    locSpsInds += myInds.low + (i,i);
  }
  //  writeln(here.id, ": ", locSpsInds);

  var locVals: [locSpsInds] real;
  for i in 0..<min(myInds.dim(0).size, myInds.dim(1).size) {
    const low = myInds.low;
    locVals[low+(i,i)] = low(0)+i + (low(1)+i)/10.0;
  }
  //  writeln(here.id, ": ", locVals);

  DS.setLocalBlock(locSpsInds);
  A.setLocalBlock(locVals);
}

writeln("DS is:");
for i in D.dim(0) {
  for j in D.dim(1) {
    write(if DS.contains(i,j) then "X" else ".");
  }
  writeln();
}
writeln();


writeln("A is:");
for i in D.dim(0) {
  for j in D.dim(1) {
    write(A[i,j], " ");
  }
  writeln();
}
