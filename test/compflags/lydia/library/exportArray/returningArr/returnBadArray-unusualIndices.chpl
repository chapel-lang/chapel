// This tests that it is forbidden to export a function that returns an array
// with indices that are not integers or uints
export proc foo(): [] int {
  var x = ["a" => 3, "b" => 6, "c" => 9];
  return x;
}
