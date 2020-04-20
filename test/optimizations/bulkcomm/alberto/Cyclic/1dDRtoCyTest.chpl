
use CyclicDist;
config const n=500;
var Space = {1..n};
const Dom1: domain(1) dmapped Cyclic(startIdx=Space.low)=Space;

var A:[Dom1] real(64);
var C:[Dom1] real(64);

var H: [1..n/2] real(64);
var F: [1..n/2] real(64);

var D6 = {1..10 by 2};
H[D6] = C[D6];
for (a,b) in zip(H[D6],C[D6]) do if (a!=b) then writeln("ERROR!!!!");
writeln();
