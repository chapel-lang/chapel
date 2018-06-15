// This tests that it is okay to return an array that doesn't violate the
// preconditions
export proc foo(): [0..3] real {
  var x: [0..3] real = [1.0, 2.0, 3.0, 4.0];
  return x;
}
