// This tests that it is okay to return an array that doesn't violate the
// preconditions
export proc foo(): [0..3] int {
  var x: [0..3] int = [1, 2, 3, 4];
  return x;
}
