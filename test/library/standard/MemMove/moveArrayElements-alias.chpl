use MemMove;

// TODO: write more tests with different kinds of arrays:
// - rank-changes
// - reindexes

proc testTwo(A: [], B: []) {
  try {
    moveArrayElements(A, B);
  } catch e: IllegalArgumentError {
    writeln("IllegalArgumentError: ", e.message());
  } catch {
    writeln("Unknown error!");
  }
}

proc testFour(A: [], regionA, B: [], regionB) {
  try {
    moveArrayElements(A, regionA, B, regionB);
  } catch e: IllegalArgumentError {
    writeln("IllegalArgumentError: ", e.message());
  } catch {
    writeln("Unknown error!");
  }
}

proc main() {
  var A : [1..10] int;

  // Trivial case
  testTwo(A, A);
  // Overlapping slices
  testTwo(A[1..7], A[4..10]);
  // Ensure we can handle a normal array and an array view
  testTwo(A, A[A.domain]);

  // Overlapping regions
  testFour(A, 1..7, A, 4..10);

  // This case should actually work, so make sure that the check doesn't
  // overzealously exclude slices to the same array.
  testTwo(A[1..5], A[6..10]);
}
