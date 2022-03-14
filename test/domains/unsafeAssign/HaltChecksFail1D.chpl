class C { var x = 0; }

// Single element array, testing if checks fire.
proc test1() {
  var D = {0..0};
  var A: [D] shared C = [new shared C()];

  writeln(A);

  // Do nothing to make sure checks fire.
  manage D.unsafeAssign({0..1}, checks=true) as mgr do;

  writeln(A);
}
test1();

