config param n1 = 100;

const Dom1D: domain(1) = [1..n1];
const SmallDom1D: domain(1) = [1..n1/2];

var A1D: [Dom1D] int;
A1D = 1;
var TA1D => A1D;
TA1D = 2;
var SmallA1D: [SmallDom1D] int;
SmallA1D = 3;
TA1D => SmallA1D;
TA1D = 4;

writeln("FAIL: We should not get here!");
writeln(A1D);
writeln(SmallA1D);
writeln(TA1D);
