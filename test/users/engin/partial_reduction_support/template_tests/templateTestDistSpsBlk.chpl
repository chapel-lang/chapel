use templates;
use utilities;
use dsiMethods;

use BlockDist;

config const N = 16;

const space = {0..#N, 0..#N, 0..#N};
const ParentDom = space dmapped Block(space);
var SparseDom: sparse subdomain(ParentDom);
var arr: [SparseDom] int;

for i in ParentDom.dim(0) {
  SparseDom += (i,i,i);
  SparseDom += (i,i,N-i-1);
  SparseDom += (i,N-i-1,N-i-1);
  SparseDom += (i,N-i-1,i);
}

for (i,j,k) in SparseDom {
  arr[i,j,k] = i*N**2 + j*N**1 + k*N**0;
}


writeln(bulkPartialReduce(arr._value, 0));
writeln();
writeln(bulkPartialReduce(arr._value, 1));
writeln();
writeln(bulkPartialReduce(arr._value, 2));
