// This tests that it is okay to return an array that doesn't violate the
// preconditions, even if it doesn't have an explicit element type
export proc foo(): [] {
  var x: [0..3] int = [1, 2, 3, 4];
  return x;
}
