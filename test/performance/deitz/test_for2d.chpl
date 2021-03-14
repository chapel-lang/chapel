const N = 1024;
const ITERS = 100;

var D = {1..N, 1..N};
var A: [D] int;

for k in 1..ITERS do
  for (i,j) in D do
    A(i,j) = i + j + k;

writeln(A(52, 52));
