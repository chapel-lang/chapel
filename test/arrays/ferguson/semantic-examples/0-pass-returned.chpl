use myrecord;
proc createArray() {
  var retA: [1..2] R;
  return retA;
}
proc consumesArray(A:[] R) {
  writeln(A[1].x);
}
consumesArray(createArray());

// It seems pretty clear that it's undesirable for the pattern above to copy
// the array elements.
