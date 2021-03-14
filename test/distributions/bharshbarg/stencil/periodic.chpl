use StencilDist;

config const debug = false;

config const maxFluff = 2;

proc test(dom : domain) {
  for i in 1..maxFluff {
    var halo : dom.rank * int;
    for j in 0..dom.rank-1 do halo(j) = i;
    test(dom, halo);
  }
}

proc test(dom : domain, halo : dom.rank * int) {
  param rank = dom.rank;

  if debug then writeln("Testing domain ", dom, " with halo ", halo);
  var Space = dom dmapped Stencil(dom, fluff=halo, periodic=true);

  var A : [Space] int;
  const n = dom.dim(0).size;
  forall idx in Space {
    var val = 0;
    for i in 0..rank-1 do val += n*idx(i);
    A[idx] = val;
  }

  A.updateFluff();
  use util;
  verifyStencil(A, debug);

  if debug then writeln();
}

test({1..10, 1..10});
test({0..9, 0..9});
test({-3..11, -3..11});
test({-20..-10, -20..-10});
test({1..10, 1..10, 1..10});
test({-10..#30, -10..#30, -10..#30} by 3);
test({-10..1, 5..24, 0..10} by 2);

// At least one dimension has no fluff
test({1..10, 1..10}, (1, 0));
test({1..10, 1..10}, (0, 1));
test({1..10, 1..10, 1..10}, (0, 1, 0));
test({1..10, 1..10, 1..10}, (1, 0, 1));


writeln("Success!");
