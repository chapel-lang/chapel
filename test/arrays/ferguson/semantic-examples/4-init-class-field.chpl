use myrecord; 
class ClassStoringArray {
  var field;
}
proc createArray() {
  var retA: [1..2] R;
  return retA;
}
proc consumesArray(A:[] R) {
  return new unmanaged ClassStoringArray(A);
}

var c = consumesArray(createArray());
delete c;
