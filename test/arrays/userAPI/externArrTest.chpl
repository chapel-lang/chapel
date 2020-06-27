export proc checkExportedArray(ref x: [] real) {
  use arrayAPItest;
  var newLen = 2*x.size;
  testArrayAPI1D("array sent from C", x, {0..3}, {0..#newLen by 2});
}
