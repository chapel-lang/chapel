use BlockDist;
use Sort, TwoArrayDistributedSampleSort;

config const N = 1000;
var A = blockDist.createArray({1..N}, int);
twoArrayDistributedSampleSort(A);
writeln(isSorted(A));

