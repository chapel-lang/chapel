use templates;
use utilities;
use dsiMethods;

config const N = 4;

const ParentDom = {0..#N, 0..#N, 0..#N};
var SparseDom: sparse subdomain(ParentDom);
var arr: [SparseDom] int;

for (i,j,k) in ParentDom {
  SparseDom += (i,j,k);
  arr[i,j,k] = i*N**2 + j*N**1 + k*N**0;
}


writeln(dsiPartialReduce_template(arr, 1));
writeln();
writeln(dsiPartialReduce_template(arr, 2));
writeln();
writeln(dsiPartialReduce_template(arr, 3));
writeln();

