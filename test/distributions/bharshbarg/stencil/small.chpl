use StencilDist;
use util;

const Dom = {0..0,0..0};
var Space = Dom dmapped new stencilDist(Dom, fluff=(1,1), periodic=true);
var A : [Space] int;
[(i,j) in Space with (ref A)] A[i,j] = i*10+j;
A.updateFluff();
verifyStencil(A);
writeln("Success!");
