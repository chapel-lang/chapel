class C {
  var x = 0;
}

proc test() {
  var D = {0..0};
  var A: [D] shared C = [new shared C(0)];
  writeln(A);

  // Initialize the same slot twice to trigger a halt.
  manage D.unsafeAssign({0..1}, checks=true) as mgr {
    mgr.initialize(A, 1, new shared C(1));
    mgr.initialize(A, 1, new shared C(1));
  }

  writeln(A);
}
test();

