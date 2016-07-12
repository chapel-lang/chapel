use myrecord; 
record RecordStoringArray{
  var field:[1..2] R;
}
proc createArray() {
  var retA: [1..2] R;
  return retA;
}
proc consumesArray(A:[1..2] R) {
  return new RecordStoringArray(A);
}
consumesArray(createArray());

