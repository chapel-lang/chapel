// NOTE: these are all cases that are commented out in typeMethods.chpl,
// which is the non-"broken" version of this future. If this future starts
// passing, delete it and uncomment the tests in typeMethods.chpl.

use BlockDist, CyclicDist, StencilDist;

const dom = {1..6, 1..6},
      rng = (1..6, 1..6);

const tls = if numLocales > 1 then [Locales[0], Locales[1]] else [Locales[0]];

config type eltType = int;
config type eltType2 = int(16);

const a: [dom] eltType = 5,
      b: [2..7, 0..10 by 2] eltType2 = 6;

var literal: eltType = -1;

writeln("\nArrays:");

testArray("Stencil value:", stencilDist.createArray((...rng), eltType, literal, targetLocales=tls));
testArray("Stencil value:", stencilDist.createArray((...rng), eltType, literal, tls));
testArray("Stencil iter:", stencilDist.createArray((...rng), eltType, [(i, j) in dom] i + j, targetLocales=tls));
testArray("Stencil iter:", stencilDist.createArray((...rng), eltType, [(i, j) in dom] i + j, tls));
testArray("Stencil array:", stencilDist.createArray((...rng), eltType, a, targetLocales=tls));
testArray("Stencil array:", stencilDist.createArray((...rng), eltType, a, tls));
testArray("Stencil array:", stencilDist.createArray((...rng), eltType, b, targetLocales=tls));
testArray("Stencil array:", stencilDist.createArray((...rng), eltType, b, tls));

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
