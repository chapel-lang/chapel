use StencilDist;

var D = {0..9 by 2 align 1} dmapped new stencilDist({1..10});
var A: [D] real;

writeln(A);
