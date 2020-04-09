config const n = 9;

const D1 = {1..n};
const D2 = {1..n};

const D = {D1, D2};

var A: [D] real = [(i,j) in D] i + j/10.0;
var B: [D1, D2] real = 0.0;

forall (i,j) in D {
  B(i,j) = i + j/10.0;
}

writeln("A is: ", A);
writeln("B is: ", B);
