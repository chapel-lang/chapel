use templates;
use utilities;
use dsiMethods;

config const N = 4;

const ParentDom = {0..#N, 0..#N, 0..#N};
var arr: [ParentDom] int;

for (i,j,k) in ParentDom {
  arr[i,j,k] = i*N**2 + j*N**1 + k*N**0;
}

writeln(partialReduce(arr._value, 1));
writeln();
writeln(partialReduce(arr._value, 2));
writeln();
writeln(partialReduce(arr._value, 3));
