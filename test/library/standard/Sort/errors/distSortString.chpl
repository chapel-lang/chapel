use BlockDist;
use Sort;
use Random;

config const N = 100;

var A = blockDist.createArray(0..<N, string);
sort(A);
writeln("A is sorted: ", isSorted(A));
// currently, this fails with compilation error,
// due to not yet having sorted out copy vs move
// in the distributed sort code
