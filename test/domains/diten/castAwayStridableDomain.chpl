config const test = 0;
var dom1 = {1..15};

proc testit(choice, stride: int, param strides) {
  if test != choice then return;

  const dom = dom1 by stride;
  type dt = domain(1, strides=strides);

  try {
    writeln(dom.tryCast(dt));
  } catch e {
    writeln("error: ", e);
  }
  writeln(dom: dt);
}

use strideKind;
testit(1, 1, negOne);
testit(2, 1, negative);
testit(3, 2, one);
testit(4, -1, one);
testit(5, -1, positive);
testit(6, -2, negOne);

var stridableDom = {1..20 by 2};
var stridableDomUnit = {1..20 by 1}: domain(1, strides=strideKind.any);
var unstridableDom = {1..10};

writeln("before cast/assign unstridableDom=", unstridableDom);        // {1..10}
writeln("before cast/assign stridableDom=", stridableDom);            // {1..20 by 2}

unstridableDom = stridableDomUnit: domain(1, strides=strideKind.one);

writeln("after cast/assign unstridableDom=", unstridableDom);         // {1..20}
writeln("after cast/assign stridableDom=", stridableDom);             // {1..20 by 2}

unstridableDom = stridableDom: domain(1, strides=strideKind.one);   // halts
writeln("finally unstridableDom=", unstridableDom);
