use LinearAlgebra;

config const m=10;

const Space = {1..m, 1..m};

var A: [Space] real;

setDiag(A, 0, 2);
setDiag(A, 1, -1);
setDiag(A, -1, -1);
setDiag(A, 6, 0.5);
setDiag(A, -6, 0.5);
setDiag(A, m - 1, 0.1);
setDiag(A, (m - 1) * (-1), 0.1);

writeln(A);
