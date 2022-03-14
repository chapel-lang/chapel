class C { var x = 0; }

config const checks = true;
config const count = 8;

iter fill(dom) {
  for (i, j) in dom do yield new shared C(0);
}

config const lo = 1;
config const hi = 256;

// 2D rectangular array with some looping.
proc test1() {
  for i in lo..hi {
    var D = {0..i, 0..i};
    var A: [D] shared C = fill(D);

    const newHi = i + 1;
    manage D.unsafeAssign({0..newHi, 0..newHi}, checks=true) as mgr do
      for (i, j) in mgr.newIndices() do
        mgr.initialize(A, (i, j), new shared C(1));

    var numOneVals = 0;
    for idx in D do if A[idx].x == 1 then numOneVals += 1;

    assert(numOneVals == (newHi + newHi + 1));
  }
}
test1();

