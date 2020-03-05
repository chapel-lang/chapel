class C {
  var xx: int = 0;
}

record R {
  var x: int = 0;
  var ptr: shared C = new shared C(0);
}

proc outPartialArray1(out arr:[1..1]) {
}

proc test1() {
  var A: [1..1] R;
  outPartialArray1(A);
}
test1();
