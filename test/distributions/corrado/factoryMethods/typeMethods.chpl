use BlockDist, CyclicDist, StencilDist;

const dom = {1..6, 1..6},
      rng = (1..6, 1..6);

const tls = if numLocales > 1 then [Locales[0], Locales[1]] else [Locales[0]];

config type eltType = int;
config type eltType2 = int(16);

const a: [dom] eltType = 5,
      b: [2..7, 0..10 by 2] eltType2 = 6;

var literal: eltType = -1;

writeln("Distributions:");

testDom("Block", blockDist.createDomain(dom));
testDom("Block", blockDist.createDomain((...rng)));
testDom("Block opts", blockDist.createDomain(dom, targetLocales=tls));
testDom("Block opts:", blockDist.createDomain((...rng), targetLocales=tls));

testDom("Stencil:", stencilDist.createDomain(dom));
testDom("Stencil:", stencilDist.createDomain((...rng)));
testDom("Stencil opts:", stencilDist.createDomain(dom, targetLocales=tls, fluff=(2, 2), periodic=true));
testDom("Stencil opts:", stencilDist.createDomain((...rng), targetLocales=tls, fluff=(2, 2), periodic=true));

testDom("Cyclic:", cyclicDist.createDomain(dom));
testDom("Cyclic:", cyclicDist.createDomain((...rng)));
testDom("Cyclic opts:", cyclicDist.createDomain(dom, targetLocales=tls));
testDom("Cyclic opts:", cyclicDist.createDomain((...rng), targetLocales=tls));

proc testDom(test: string, D: domain(?)) {
  writeln("\n", test);
  var A: [D] int;
  forall a in A do a = here.id;
  writeln(A);
  writeln(D);
  writeln(D.type:string);
}

writeln("\nArrays:");

testArray("Block:", blockDist.createArray(dom, eltType));
testArray("Block:", blockDist.createArray((...rng), eltType));
testArray("Block opts:", blockDist.createArray(dom, eltType, targetLocales=tls));
testArray("Block opts:", blockDist.createArray((...rng), eltType, targetLocales=tls));
testArray("Block value:", blockDist.createArray(dom, eltType, literal));
testArray("Block value:", blockDist.createArray((...rng), eltType, literal));
testArray("Block iter:", blockDist.createArray(dom, eltType, [(i, j) in dom] i + j));
testArray("Block iter:", blockDist.createArray((...rng), eltType, [(i, j) in dom] i + j));
testArray("Block array:", blockDist.createArray(dom, eltType, a));
testArray("Block array:", blockDist.createArray(dom, eltType, b));
testArray("Block array:", blockDist.createArray((...rng), eltType, a));
testArray("Block array:", blockDist.createArray((...rng), eltType, b));

testArray("Stencil:", stencilDist.createArray(dom, eltType));
testArray("Stencil:", stencilDist.createArray((...rng), eltType));
testArray("Stencil opts:", stencilDist.createArray(dom, eltType, targetLocales=tls, fluff=(2, 2), periodic=true));
testArray("Stencil opts:", stencilDist.createArray((...rng), eltType, targetLocales=tls, fluff=(2, 2), periodic=true));
testArray("Stencil value:", stencilDist.createArray(dom, eltType, literal));
testArray("Stencil value:", stencilDist.createArray((...rng), eltType, literal));
testArray("Stencil iter:", stencilDist.createArray(dom, eltType, [(i, j) in dom] i + j));
testArray("Stencil iter:", stencilDist.createArray((...rng), eltType, [(i, j) in dom] i + j));
testArray("Stencil array:", stencilDist.createArray(dom, eltType, a));
testArray("Stencil array:", stencilDist.createArray(dom, eltType, b));
testArray("Stencil array:", stencilDist.createArray((...rng), eltType, a));
testArray("Stencil array:", stencilDist.createArray((...rng), eltType, b));

testArray("Cyclic:", cyclicDist.createArray(dom, eltType));
testArray("Cyclic:", cyclicDist.createArray((...rng), eltType));
testArray("Cyclic opts:", cyclicDist.createArray(dom, eltType, targetLocales=tls));
testArray("Cyclic opts:", cyclicDist.createArray((...rng), eltType, targetLocales=tls));
testArray("Cyclic value:", cyclicDist.createArray(dom, eltType, literal));
testArray("Cyclic value:", cyclicDist.createArray((...rng), eltType, literal));
testArray("Cyclic iter:", cyclicDist.createArray(dom, eltType, [(i, j) in dom] i + j));
testArray("Cyclic iter:", cyclicDist.createArray((...rng), eltType, [(i, j) in dom] i + j));
testArray("Cyclic array:", cyclicDist.createArray(dom, eltType, a));
testArray("Cyclic array:", cyclicDist.createArray(dom, eltType, b));
testArray("Cyclic array:", cyclicDist.createArray((...rng), eltType, a));
testArray("Cyclic array:", cyclicDist.createArray((...rng), eltType, b));


proc testArray(test: string, in A: [] int) {
  writeln("\n", test);
  forall a in A do a += here.id;
  writeln(A);
  writeln(A.domain);
  writeln(A.type:string);
}
proc testArray(test: string, in A: [] atomic int) {
  writeln("\n", test);
  forall a in A do a.add(here.id);
  writeln(A);
  writeln(A.domain);
  writeln(A.type:string);
}
