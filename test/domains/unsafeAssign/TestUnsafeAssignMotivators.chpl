class C { var x = 0; }

config const checks = true;
config const count = 8;

// Single element array, no looping.
proc test1() {
  var D = {0..0};
  var A: [D] shared C = [new shared C()];

  writeln(A);

  manage D.unsafeAssign({0..1}, checks=true) as mgr do
    mgr.initialize(A, 1, new shared C(1));

  writeln(A);
}
test1();

// Resize a domain many times in a looping pattern. Utilize helper
// methods on the manager to make iterating over new indices easy.
proc test2() {
  var D = {0..0};
  var A: [D] shared C = [new shared C()];

  for i in 0..#count {
    manage D.unsafeAssign({0..i}, checks=true) as mgr {
      for idx in mgr.newIndices() {
        mgr.initialize(A, idx, new shared C(idx));
      }
    }
    writeln(A);
  }
}
test2();

