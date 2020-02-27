class C {
  var xx: int = 0;
}

record R {
  var x: int = 0;
  var ptr: shared C = new shared C(0);
}

proc outArrayDomainMismatch2(out arr) {
  var B: [1..2] R;
  arr = B;
}

proc test3() {
  var A: [1..1] R;
  outArrayDomainMismatch2(A);
}
test3();
