class C {
  var xx: int = 0;
}

record R {
  var x: int = 0;
  var ptr: shared C = new shared C(0);
}

proc inoutPartialArray1(inout arr:[1..1]) {
}

proc test11b() {
  var A: [1..1] R;
  inoutPartialArray1(A);
}
test11b();

proc inoutPartialArray2(inout arr:[1..1]) {
  var A: [1..1] R;
  arr = A;
}

proc test11c() {
  var A: [1..1] R;
  inoutPartialArray2(A);
}
test11c();
