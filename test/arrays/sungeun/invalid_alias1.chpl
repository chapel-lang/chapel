config param n1 = 100;

const Dom1D: domain(1) = [1..n1];
const BigDom1D: domain(1) = [1..n1*2];

var A1D: [Dom1D] int;
A1D = 1;
var TA1D => A1D;
TA1D = 2;
var BigA1D: [BigDom1D] int;
BigA1D = 3;
TA1D => BigA1D;
TA1D = 4;

writeln("FAIL: We should not get here!");
writeln(A1D);
writeln(BigA1D);
writeln(TA1D);
