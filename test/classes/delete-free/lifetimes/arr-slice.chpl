pragma "safe"
module arrslice {

proc refIdentity(ref x) ref {
  return x;
}


pragma "return not owned"
pragma "no copy return"
proc arraySliceIdentity(x) {
  return x;
}

pragma "fn returns aliasing array"
pragma "no copy return"
proc badReturnBorrowedArraySlice() {
  var A:[1..10] int;

  return arraySliceIdentity(A[1..2]);
  // A and its elements are destroyed here
}

proc badReturnBorrowedArraySliceRef() ref {
  var A:[1..10] int;

  return refIdentity(A[1..2]);
  // A and its elements are destroyed here
}



proc test() {
  var v1 = badReturnBorrowedArraySlice();
  var v2 = badReturnBorrowedArraySliceRef();
  writeln(v1);
  writeln(v2);
}

test();
}
