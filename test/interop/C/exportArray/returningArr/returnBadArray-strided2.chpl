// This tests that it is forbidden to export a function that returns a strided
// array
export proc foo(): [0..7 by 2] int {
  var x: [0..7 by 2] int = [1, 2, 3, 4];
  return x;
}
