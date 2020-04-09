use BlockDist;
use LayoutCS;

config const N = 8;
config type sparseLayoutType = DefaultDist;

const ParentDom = {0..#N, 0..#N} dmapped Block({0..#N, 0..#N},
    sparseLayoutType=sparseLayoutType);

var SparseDom: sparse subdomain(ParentDom);
var SparseMat: [SparseDom] int;

var inds: [{0..#2*N}] 2*int;

//left diagonal
for i in 0..#N {
  inds[i] = (i, i);
}

//right diagonal
for i in 0..#N {
  inds[N+i] = (i, N-i-1);
}

var copyInds = inds;

/*SparseDom.bulkAdd(inds, false, false, false);*/
SparseDom += inds;
for (i1, i2) in zip(inds, copyInds) do 
  if i1!=i2 then halt("Broken");
