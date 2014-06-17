config const n = 10;
const D = {1..n, 1..n};
var A: [(i,j) in D] int = i*10 + j;
A.IRV = -1;

writeln("A is: ", A);
