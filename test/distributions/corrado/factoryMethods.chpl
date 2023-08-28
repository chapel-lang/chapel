use BlockDist, CyclicDist, StencilDist;

const dom = {1..6, 1..6},
      rng = (1..6, 1..6);

const tls = if numLocales > 1 then [Locales[0], Locales[1]] else [Locales[0]];

const a : [dom] int = 5;

writeln("Distributions:");

testDom("Block", Block.createDomain(dom));
testDom("Block", Block.createDomain((...rng)));
testDom("Block opts", Block.createDomain(dom, targetLocales=tls));
testDom("Block opts:", Block.createDomain((...rng), targetLocales=tls));
testDom("Stencil:", Stencil.createDomain(dom));
testDom("Stencil:", Stencil.createDomain((...rng)));
testDom("Stencil opts:", Stencil.createDomain(dom, targetLocales=tls, fluff=(2, 2), periodic=true));
testDom("Stencil opts:", Stencil.createDomain((...rng), targetLocales=tls, fluff=(2, 2), periodic=true));
testDom("Cyclic:", Cyclic.createDomain(dom));
testDom("Cyclic:", Cyclic.createDomain((...rng)));
testDom("Cyclic opts:", Cyclic.createDomain(dom, targetLocales=tls));
testDom("Cyclic opts:", Cyclic.createDomain((...rng), targetLocales=tls));

proc testDom(test: string, D: domain) {
  writeln("\n", test);
  var A: [D] int;
  forall a in A do a = here.id;
  writeln(A);
}

writeln("\nArrays:");

testArray("Block:", Block.createArray(dom, int));
testArray("Block:", Block.createArray((...rng), int));
testArray("Block opts:", Block.createArray(dom, int, targetLocales=tls));
testArray("Block opts:", Block.createArray((...rng), int, targetLocales=tls));
testArray("Block value:", Block.createArray(dom, int, -1));
testArray("Block value:", Block.createArray((...rng), int, -1));
testArray("Block iter:", Block.createArray(dom, int, [(i, j) in dom] i + j));
testArray("Block iter:", Block.createArray((...rng), int, [(i, j) in dom] i + j));
testArray("Block array:", Block.createArray(a));

testArray("Stencil:", Stencil.createArray(dom, int));
testArray("Stencil:", Stencil.createArray((...rng), int));
testArray("Stencil opts:", Stencil.createArray(dom, int, targetLocales=tls, fluff=(2, 2), periodic=true));
testArray("Stencil opts:", Stencil.createArray((...rng), int, targetLocales=tls, fluff=(2, 2), periodic=true));
testArray("Stencil value:", Stencil.createArray(dom, int, -1));
testArray("Stencil value:", Stencil.createArray((...rng), int, -1));
testArray("Stencil iter:", Stencil.createArray(dom, int, [(i, j) in dom] i + j));
testArray("Stencil iter:", Stencil.createArray((...rng), int, [(i, j) in dom] i + j));
testArray("Stencil array:", Stencil.createArray(a));

testArray("Cyclic:", Cyclic.createArray(dom, int));
testArray("Cyclic:", Cyclic.createArray((...rng), int));
testArray("Cyclic opts:", Cyclic.createArray(dom, int, targetLocales=tls));
testArray("Cyclic opts:", Cyclic.createArray((...rng), int, targetLocales=tls));
testArray("Cyclic value:", Cyclic.createArray(dom, int, -1));
testArray("Cyclic value:", Cyclic.createArray((...rng), int, -1));
testArray("Cyclic iter:", Cyclic.createArray(dom, int, [(i, j) in dom] i + j));
testArray("Cyclic iter:", Cyclic.createArray((...rng), int, [(i, j) in dom] i + j));
testArray("Cyclic array:", Cyclic.createArray(a));

proc testArray(test: string, in A: [] int) {
  writeln("\n", test);
  forall a in A do a += here.id;
  writeln(A);
}
