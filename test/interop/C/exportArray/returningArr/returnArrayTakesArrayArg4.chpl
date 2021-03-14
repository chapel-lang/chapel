// This tests that it is okay to return an array that doesn't violate the
// preconditions, even when we took it in as an argument
export proc foo(const ref arr: [] int): [] int {
  return arr;
}
