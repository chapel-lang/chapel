use BlockDist;
use Sort;
use Random;

config const N = 1_000_000;

// some basic checks of distributed sort through 'proc sort'
var A = blockDist.createArray(0..<N, int);
fillRandom(A, seed=1);
sort(A, stable=false);
writeln("A is sorted: ", isSorted(A));

var B = blockDist.createArray(1..N, int);
fillRandom(B, seed=1);
sort(B, stable=false);
writeln("B is sorted: ", isSorted(B));

var C = blockDist.createArray(0..<N, int);
fillRandom(C, seed=1);
sort(C, stable=true);
writeln("C is sorted: ", isSorted(C));

var D = blockDist.createArray(1..N, int);
fillRandom(D, seed=1);
sort(D, stable=true);
writeln("D is sorted: ", isSorted(D));

// TODO: check comparison sort
// TODO: check stable with comparators
