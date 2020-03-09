pragma "safe"
module arrslice {

class MyClass {
  var x:int;
}

proc refIdentity(ref x) ref {
  return x;
}

proc returnsArraySliceCopy() {
  var A:[1..10] borrowed MyClass?;

  return refIdentity(A[1..2]);
}

// The array type MyClass here indicates a borrow.
// The A implementation includes an owning ptr, which
// is confusing analysis in this case. We're conservatively
// assuming that A[i] might return something that could be
// destroyed by the array, but that's not the case here.
// This issue is related to get_coeffs in MRA.chpl.
proc returnsArrayElt1(i) {
  var A:[1..10] borrowed MyClass?;

  return A[i];
}

proc returnsArrayElt2(i, j) {
  var A:[1..10, 1..10] borrowed MyClass?;

  return refIdentity(A[i,j]);
}



proc test() {
  var v3 = returnsArraySliceCopy();
  var v4 = returnsArrayElt1(1);
  var v5 = returnsArrayElt2(1,2);
  writeln(v3);
  writeln(v4);
  writeln(v5);
}

test();
}
