use StencilDist;
use util;

const Dom = {0..0,0..0};
var Space = Dom dmapped Stencil(Dom, fluff=(1,1), periodic=true);
var A : [Space] int;
[(i,j) in Space] A[i,j] = i*10+j;
A.updateFluff();
verifyStencil(A);
writeln("Success!");
