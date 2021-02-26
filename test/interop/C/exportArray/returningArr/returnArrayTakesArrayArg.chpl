// This tests that it is okay to return an array that doesn't violate the
// preconditions, even when we took in an argument
export proc foo(const ref arr: [] int): [] int {
  var x: [arr.domain] int = 0;
  return x;
}
