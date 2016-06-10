use myrecord;
proc createArray() {
  var retA: [1..2] R;
  return retA;
}
proc consumesArray(A:[] R) {
  writeln(A[1]);
}
consumesArray(createArray());
