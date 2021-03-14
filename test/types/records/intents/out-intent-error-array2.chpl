class C {
  var xx: int = 0;
}

record R {
  var x: int = 0;
  var ptr: shared C = new shared C(0);
}

proc outArrayDomainMismatch(out arr:[1..2] R) {
}

proc test2() {
  var A: [1..1] R;
  outArrayDomainMismatch(A);
}
test2();
