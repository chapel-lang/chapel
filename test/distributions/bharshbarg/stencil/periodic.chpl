use StencilDist;

config const debug = false;

proc test(dom : domain, hval : int) {
  param rank = dom.rank;
  var halo : rank*int;
  for i in 1..rank do halo(i) = hval;

  if debug then writeln("Testing domain ", dom, " with halo ", halo);
  var Space = dom dmapped Stencil(dom, fluff=halo, periodic=true);

  var A : [Space] int;
  const n = dom.dim(1).size;
  forall idx in Space {
    var val = 0;
    for i in 1..rank do val += n*idx(i);
    A[idx] = val;
  }

  A.updateFluff();
  use util;
  verifyStencil(A, debug);

  if debug then writeln();
}

const ones = {1..10, 1..10};
const zeroes = {0..9, 0..9};
const mixed = {-3..11, -3..11};
const neg = {-20..-10, -20..-10};
const three = {1..10, 1..10, 1..10};

test(ones, 1);
test(ones, 2);

test(zeroes, 1);
test(zeroes, 2);

test(mixed, 1);
test(mixed, 2);

test(neg, 1);
test(neg, 2);

test(three, 1);
test(three, 2);

writeln("Success!");
