
use BlockDist;
config const n=500;
var DA = {1..n};
var Dom1 = DA dmapped Block(DA);

var A:[Dom1] real(64);
var C:[Dom1] real(64);

var H: [1..n/2] real(64);
var F: [1..n/2] real(64);

var D6 = {1..10 by 2};
H[D6] = C[D6];
for (a,b) in zip(H[D6],C[D6]) do if (a!=b) then writeln("ERROR!!!!");
writeln("");
