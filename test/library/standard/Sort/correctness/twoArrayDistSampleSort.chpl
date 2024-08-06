use BlockDist;
use Sort, TwoArrayDistributedSampleSort;
use Random;

config const N = 1000;
var A = blockDist.createArray({1..N}, int);
fillRandom(A);
twoArrayDistributedSampleSort(A);
writeln(isSorted(A));

