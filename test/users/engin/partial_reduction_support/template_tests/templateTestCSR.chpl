use templates;
use utilities;
use dsiMethods;

use LayoutCSR;

config const N = 4;

const ParentDom = {0..#N, 0..#N};
var SparseDom: sparse subdomain(ParentDom) dmapped CSR();
var arr: [SparseDom] int;

for (i,j) in ParentDom {
  SparseDom += (i,j);
  arr[i,j] = i*N**1 + j*N**0;
}

writeln(partialReduce(arr._value, 1));
writeln();
writeln(partialReduce(arr._value, 2));
