config const n = 4;

const D = [1..n, 1..n];

var A: [(i,j) in D] real = i + j/10.0;

writeln("A is: ", A);

for (aRow, aCol) in [ A[1, ..], A[.., 1] ] {
  writeln("aRow is: ", aRow, "; aCol is: ", aCol);
}
