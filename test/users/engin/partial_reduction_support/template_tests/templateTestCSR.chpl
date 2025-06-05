use templates;
use utilities;
use dsiMethods;

use CompressedSparseLayout;

config const N = 4;

const ParentDom = {0..#N, 0..#N};
var SparseDom: sparse subdomain(ParentDom) dmapped new csrLayout();
var arr: [SparseDom] int;

for (i,j) in ParentDom {
  SparseDom += (i,j);
  arr[i,j] = i*N**1 + j*N**0;
}

writeln(partialReduce(arr._value, 0));
writeln();
writeln(partialReduce(arr._value, 1));
