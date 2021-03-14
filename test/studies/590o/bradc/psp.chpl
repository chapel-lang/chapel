config const n = 4;

const D = {1..n, 1..n};

var A: [D] real = [(i,j) in D] i + j/10.0;

writeln("A is: ", A);

for aRow in A[1, ..] {
  for aCol in A[.., 1] {
    writeln("aRow is: ", aRow, "; aCol is: ", aCol);
  }
}
