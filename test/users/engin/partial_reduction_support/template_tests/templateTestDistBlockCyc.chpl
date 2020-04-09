use templates;
use utilities;
use dsiMethods;

use BlockCycDist;

config const N = 4;

const space = {0..#N, 0..#N};
const ParentDom = space dmapped BlockCyclic(space.low, blocksize=(1,1));
var arr: [ParentDom] int;

for (i,j) in ParentDom {
  arr[i,j] = i*N**1 + j*N**0;
}

writeln(bulkPartialReduce(arr._value, 0));
writeln();
writeln(bulkPartialReduce(arr._value, 1));
