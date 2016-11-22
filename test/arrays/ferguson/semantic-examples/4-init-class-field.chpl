use myrecord; 
class ClassStoringArray {
  var field;
}
proc createArray() {
  var retA: [1..2] R;
  return retA;
}
proc consumesArray(A:[] R) {
  return new ClassStoringArray(A);
}

var c = consumesArray(createArray());
delete c;
