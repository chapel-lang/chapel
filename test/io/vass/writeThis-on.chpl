// Illustrate the issue by using a customized writeThis method.

class C {
  const home = here.id;
  override proc writeThis(f) throws { f.write("here=", here.id, " home=", home); }
}

for l in Locales do on l do { const c = new borrowed C(); writeln(c); }
writeln();


// Illustrate the issue with a replicated array.

use DimensionalDist2D, ReplicatedDim, BlockCycDim;

const dim1 = new ReplicatedDim(numLocales);
const dim2 = new BlockCyclicDim(1, 1, 2);
const dmp = newDimensionalDist2D(dim1, dim2, Locales);

const ix = (1, 1);
const D = {1..1, 1..1};
const R = D dmapped new dmap(dmp);
var A: [R] int;

writeln("initializing");
for l in Locales do on l do A[ix] = here.id;

writeln("printing element directly");
for l in Locales do on l do writeln(A[ix]);

writeln("printing array directly");
for l in Locales do on l do writeln(A);

writeln("printing array via temp");
for l in Locales do on l do { const temp: [D] int = A; writeln(temp); }
