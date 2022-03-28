class C { var x = 0; }

proc test() {
  var D = {0..0};
  var A: [D] shared C = [new shared C(0)];

  // Initialize a bad array index.
  manage D.unsafeAssign({0..1}, checks=true) as mgr do
    mgr.initialize(A, 2, new shared C(1));

}
test();

