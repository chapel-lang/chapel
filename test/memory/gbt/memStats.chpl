use BlockDist;
config const n = numLocales * 2**20;
const D: domain(1) dmapped Block(boundingBox={1..n}) = {1..n};
type T = int(8);
var A: [D] T;
config const i = n / 2;
config const val = 22:T;
A(i) = val;
writeln(A(i));
