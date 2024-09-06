use StencilDist;

const D = {1..10} dmapped new stencilDist({1..10});
var A: [D] real = [i in D] i;

A.updateFluff();
writeln(here.id, ": ", A.localSubdomain());
writeln(A.localAccess(6));
