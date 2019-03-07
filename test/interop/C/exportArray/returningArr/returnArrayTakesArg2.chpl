// This tests that it is okay to return an array that doesn't violate the
// preconditions, even when we took in an argument
export proc foo(size: int): [0..#size] int {
  var x: [0..#size] int;
  return x;
}
