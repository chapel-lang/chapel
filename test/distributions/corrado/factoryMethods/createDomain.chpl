use BlockDist, CyclicDist, StencilDist;

const dom = {1..6, 1..6},
      rng = (1..6, 1..6);

const b = new blockDist(dom),
      c = new cyclicDist(dom.lowBound),
      s = new stencilDist(dom, fluff=(1,1));

testCreateDomainDom("Block:", b);
testCreateDomainDom("Cyclic:", c);
testCreateDomainDom("Stencil:", s);
testCreateDomainRng("Block:", b);
testCreateDomainRng("Cyclic:", c);
testCreateDomainRng("Stencil:", s);

proc testCreateDomainDom(test: string, dist) {
  var D = dist.createDomain(dom);
  var A: [D] int;
  forall a in A do a = here.id;
  writeln("\n", test);
  writeln(A);
}

proc testCreateDomainRng(test: string, dist) {
  var D = dist.createDomain((...rng));
  var A: [D] int;
  forall a in A do a = here.id;
  writeln("\n", test);
  writeln(A);
}
