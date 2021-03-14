use BlockDist;
use LayoutCS;

config const N = 8;
config type sparseLayoutType = DefaultDist;
const ParentDom = {0..#N, 0..#N} dmapped Block({0..#N, 0..#N},
    sparseLayoutType=sparseLayoutType);
var SparseDom: sparse subdomain(ParentDom);
var SparseMat: [SparseDom] int;
var diagInds: [{0..#N*2}] 2*int;

for i in ParentDom.dim(0) {
  diagInds[2*i] = (i,i);
  diagInds[2*i+1] = (i, N-1-i);
}

SparseDom += diagInds;

forall i in SparseMat do i = i.locale.id+1;

for i in ParentDom.dim(0) {
  for j in ParentDom.dim(1) {
    write(SparseMat[i, j], " ");
  }
  writeln();
}
