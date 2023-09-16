use BlockDist, CyclicDist, StencilDist;

const dom = {1..6, 1..6},
      rng = (1..6, 1..6);

const tls = if numLocales > 1 then [Locales[0], Locales[1]] else [Locales[0]];

const a: [dom] int = 5,
      b: [2..7, 0..10 by 2] int(16) = 6;

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

testArray("Block:", blockDist.createArray(dom, int));
testArray("Block:", blockDist.createArray((...rng), int));
testArray("Block opts:", blockDist.createArray(dom, int, targetLocales=tls));
testArray("Block opts:", blockDist.createArray((...rng), int, targetLocales=tls));
testArray("Block value:", blockDist.createArray(dom, int, -1));
testArray("Block value:", blockDist.createArray((...rng), int, -1));
testArray("Block iter:", blockDist.createArray(dom, int, [(i, j) in dom] i + j));
testArray("Block iter:", blockDist.createArray((...rng), int, [(i, j) in dom] i + j));
testArray("Block array:", blockDist.createArray(dom, int, a));
testArray("Block array:", blockDist.createArray(dom, int, b));
testArray("Block array:", blockDist.createArray((...rng), int, a));
testArray("Block array:", blockDist.createArray((...rng), int, b));

testArray("Stencil:", stencilDist.createArray(dom, int));
testArray("Stencil:", stencilDist.createArray((...rng), int));
testArray("Stencil opts:", stencilDist.createArray(dom, int, targetLocales=tls, fluff=(2, 2), periodic=true));
testArray("Stencil opts:", stencilDist.createArray((...rng), int, targetLocales=tls, fluff=(2, 2), periodic=true));
testArray("Stencil value:", stencilDist.createArray(dom, int, -1));
testArray("Stencil value:", stencilDist.createArray((...rng), int, -1));
testArray("Stencil iter:", stencilDist.createArray(dom, int, [(i, j) in dom] i + j));
testArray("Stencil iter:", stencilDist.createArray((...rng), int, [(i, j) in dom] i + j));
testArray("Stencil array:", stencilDist.createArray(dom, int, a));
testArray("Stencil array:", stencilDist.createArray(dom, int, b));
testArray("Stencil array:", stencilDist.createArray((...rng), int, a));
testArray("Stencil array:", stencilDist.createArray((...rng), int, b));

testArray("Cyclic:", cyclicDist.createArray(dom, int));
testArray("Cyclic:", cyclicDist.createArray((...rng), int));
testArray("Cyclic opts:", cyclicDist.createArray(dom, int, targetLocales=tls));
testArray("Cyclic opts:", cyclicDist.createArray((...rng), int, targetLocales=tls));
testArray("Cyclic value:", cyclicDist.createArray(dom, int, -1));
testArray("Cyclic value:", cyclicDist.createArray((...rng), int, -1));
testArray("Cyclic iter:", cyclicDist.createArray(dom, int, [(i, j) in dom] i + j));
testArray("Cyclic iter:", cyclicDist.createArray((...rng), int, [(i, j) in dom] i + j));
testArray("Cyclic array:", cyclicDist.createArray(dom, int, a));
testArray("Cyclic array:", cyclicDist.createArray(dom, int, b));
testArray("Cyclic array:", cyclicDist.createArray((...rng), int, a));
testArray("Cyclic array:", cyclicDist.createArray((...rng), int, b));


proc testArray(test: string, in A: [] int) {
  writeln("\n", test);
  forall a in A do a += here.id;
  writeln(A);
  writeln(A.domain);
  writeln(A.type:string);
}
