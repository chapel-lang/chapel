// A test of the QuickSort function in the Sort module

use Sort;
use Random;

config const size = 4096;
config const seed = 27;

proc main() {
  var A: [1..size] int;
  var rands: [1..size] real;
  var randomStream = new RandomStream(seed);

  // Fill A with random int values in [0, 99999]
  randomStream.fillRandom(rands);
  delete randomStream;
  [i in 1..size] A(i) = (100000 * rands(i)):int;

  QuickSort(A);

  // Check that A is sorted
  [i in 1..size-1] {
    if A(i) > A(i+1) then
      writeln(A(i), " ", A(i+1));
  }
}
