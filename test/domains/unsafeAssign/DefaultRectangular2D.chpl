class C { var x = 0; }

config const checks = true;
config const count = 8;

iter fill(dom) {
  for (i, j) in dom do yield new shared C(j);
}

// 2D rectangular array with some looping.
proc test1() {
  var D = {0..1, 0..1};
  var A: [D] shared C = fill(D);

  writeln(A);

  manage D.unsafeAssign({0..1, 0..2}, checks=true) as mgr do
    for (i, j) in mgr.newIndices() do
      mgr.initialize(A, (i, j), new shared C(j));

  writeln(A);
}
test1();

