use Random;
use Sort;
use Math;
use BlockDist;

config const n = 1000000;
config const seed = 17;
config const bound = 2**32;

proc testLocal() {
  writeln("testing local sorting");

  var D = {0..#n};
  var A: [D] int;

  fillRandom(A, seed=seed);
  A = abs(A) % bound;
  writeln("TwoArrayRadixSort.twoArrayRadixSort");
  var AA = A;
  TwoArrayRadixSort.twoArrayRadixSort(AA);
  writeln("Result is sorted? ", isSorted(AA));

  writeln("standard sort");
  var A2 = A;
  sort(A2);
  writeln("Result is sorted? ", isSorted(A2));
  writeln();
}
testLocal();

proc testDist() {
  writeln("testing distributed sorting");
  var D = {0..#n};
  var DD: domain(1) dmapped Block(boundingBox=D) = D;
  var A: [DD] int;

  fillRandom(A, seed=seed);
  A = abs(A) % bound;
  writeln("TwoArrayRadixSort.twoArrayRadixSort");
  var AA = A;
  TwoArrayRadixSort.twoArrayRadixSort(AA);
  writeln("Result is sorted? ", isSorted(AA));

  writeln("standard sort");
  var A2:[D] int = A;
  sort(A2);
  writeln("Result is sorted? ", isSorted(A2));
  writeln();
}
testDist();
