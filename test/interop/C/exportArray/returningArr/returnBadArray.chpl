// This tests that it is an error to return an array with a different lower
// bounds than expected
export proc foo(): [] int {
  var x: [1..4] int = [1, 2, 3, 4]; // explicitly make the lower bound 1
  return x;
}
