use templates;
use utilities;
use dsiMethods;

use BlockDist;

config const N = 4;

const space = {0..#N, 0..#N, 0..#N};
const ParentDom = space dmapped Block(space);
var arr: [ParentDom] int;

for (i,j,k) in ParentDom {
  arr[i,j,k] = i*N**2 + j*N**1 + k*N**0;
}

writeln(bulkPartialReduce(arr._value, 0));
writeln();
writeln(bulkPartialReduce(arr._value, 1));
writeln();
writeln(bulkPartialReduce(arr._value, 2));
