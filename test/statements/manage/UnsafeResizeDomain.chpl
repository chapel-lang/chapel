use Memory.Initialization;

class C { var x = 0; }

config const checks = true;
config const count = 8;

proc test1() {
  var D = {0..0};
  var A: [D] shared C = [new shared C()];

  writeln(A);

  manage D.unsafeResize({0..1}, checks=checks) {
    moveInitialize(A[1], new shared C(1));
  }

  writeln(A);
}
test1();

proc test2() {
  var D = {0..0};
  var A: [D] shared C = [new shared C()];

  for i in 0..#count {
    const old = D;
    manage D.unsafeResize({0..i}, checks=checks) {
      for idx in D do
        if !old.contains(idx) {
          moveInitialize(A[idx], new shared C(idx));
        }
    }
    writeln(A);
  }
}
test2();
