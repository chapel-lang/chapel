record RecordStoringArray{ var field; }
proc createArray() {
  var retA: [1..10000] real;
  return retA;
}
proc consumesArray(A:[] real) {
  return new RecordStoringArray(A);
}
consumesArray(createArray());

