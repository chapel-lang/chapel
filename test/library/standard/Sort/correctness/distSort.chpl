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


record myComparator: relativeComparator {
  proc compare(a, b) {
    if a < b then return -1;
    else if a > b then return 1;
    else return 0;
  }
}
var ELoc:[0..<N] int;
fillRandom(ELoc, seed=1);
sort(ELoc, comparator=new myComparator());
writeln("ELoc is sorted: ", isSorted(ELoc));

var EDist = blockDist.createArray(0..<N, int);
fillRandom(EDist, seed=1);
sort(EDist, comparator=new myComparator());
writeln("EDist is sorted: ", isSorted(EDist));
