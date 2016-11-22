use myrecord; 
record RecordStoringArray{
  var field;
}
proc createArray() {
  var retA: [1..2] R;
  return retA;
}
proc consumesArray(A:[] R) {
  return new RecordStoringArray(A);
}
consumesArray(createArray());

