config param n1 = 100;
config param n2 = 20;

const Dom1D: domain(1) = [1..n1];
const Dom2D: domain(2) = [1..n2, 1..n2];

var A1D: [Dom1D] int;
A1D = 1;
var TA1D => A1D;
TA1D = 2;
var A2D: [Dom2D] int;
A2D = 3;
TA1D => A2D(Dom2D.dim(1), n2);
TA1D = 4;

writeln("FAIL: We should not get here!");
writeln(A1D);
writeln(A2D);
writeln(TA1D);
