// This tests that it is an error to return an array with a different lower
// bounds than expected
export proc foo(): [1..4] int {
  var x = [1, 2, 3, 4]; // lower bound is 0 (because that's the default)
  return x;   // when returning the array, the declared indices [1..4] are used
}
