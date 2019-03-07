// This tests that it is forbidden to export a function that returns an array
// with a rank that isn't 1
export proc foo(): [] int {
  var x: [0..3, 0..3] int;
  return x;
}
