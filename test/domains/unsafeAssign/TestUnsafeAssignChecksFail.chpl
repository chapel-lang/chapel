class C { var x = 0; }

config const checks = true;
config const count = 8;

// Single element array, no looping.
proc test1() {
  var D = {0..0};
  var A: [D] shared C = [new shared C()];

  writeln(A);

  // Resize to 3 elements, but only initialize one.
  manage D.unsafeAssign({0..2}, checks=true) as mgr do
    mgr.initialize(A, 1, new shared C(1));

  writeln(A);
}
test1();
