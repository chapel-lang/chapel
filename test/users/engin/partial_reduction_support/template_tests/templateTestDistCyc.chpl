use templates;
use utilities;
use dsiMethods;

use CyclicDist;

config const N = 4;

const space = {0..#N, 0..#N, 0..#N};
const ParentDom = space dmapped Cyclic(space.low);
var arr: [ParentDom] int;

for (i,j,k) in ParentDom {
  arr[i,j,k] = i*N**2 + j*N**1 + k*N**0;
}

writeln(bulkPartialReduce(arr._value, 1));
writeln();
writeln(bulkPartialReduce(arr._value, 2));
writeln();
writeln(bulkPartialReduce(arr._value, 3));
