proc createArray() {
  var retA: [1..10000] real;
  return retA;
}
proc consumesArray(A:[] real) {
  writeln(A[1]);
}
consumesArray(createArray());
