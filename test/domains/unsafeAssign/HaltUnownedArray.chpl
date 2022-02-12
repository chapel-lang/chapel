class C { var x = 0; }

proc test() {
  var D = {0..0};
  var A: [D] shared C = [new shared C(0)];

  // This array is not declared over 'D'.
  var B: [0..0] shared C = [new shared C(0)];

  // Should halt because 'B' is not declared over 'D'.
  manage D.unsafeAssign({0..1}, checks=true) as mgr {
    mgr.initialize(A, 1, new shared C(1));
    mgr.initialize(B, 1, new shared C(1));
  }
}
test();

