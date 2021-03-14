// This tests that it is an error to return an array with a different lower
// bounds than expected
export proc foo(): [1..4] int {
  var x = [1, 2, 3, 4]; // lower bounds is 1 (because that's the default)
  return x;
}
