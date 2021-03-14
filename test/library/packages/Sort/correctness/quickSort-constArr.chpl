// Here's a variation that pushes on the flaw found by
// quickSort-fixSeed.chpl, which is that the pivot == the
// Data(loptr) == Data(hiptr) by making the whole array
// have the same value.

use Sort;
use Random;

config const size = 4096;

proc main() {
  var A: [1..size] int = 12345;

  QuickSort.quickSort(A);

  var passed = true;
  // Check that A is sorted
  [i in 1..size with (ref passed)] {
    if A(i) != 12345 then {
      writeln(A(i));
      passed = false;
    }
  }
  if (passed) {
    writeln("Verification passed");
  } else {
    writeln("Verification failed");
  }
}
