use LinearAlgebra;

var matA : [1..3, 1..3, 1..3] real;
var matB : [1..3, 1..3, 1..3] real;

forall (i,j,k) in matA.domain {
  matA[i,j,k] = i*10.0 + j + k/10.0;
}

writeln("matA is:\n", matA, "\n");

var val = -1.0;
for (i,j,k) in matB.domain {
  matB[i,j,k] = val;
  val -= 1.0;
}

writeln("matB is:\n", matB, "\n");

writeln(dot(matA[1, .., ..], matB[1, .., ..]), "\n");


const matC = matA[1, .., ..];
const matD = matB[1, .., ..];

writeln(dot(matC, matD));
