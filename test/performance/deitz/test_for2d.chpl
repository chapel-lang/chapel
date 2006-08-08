const N = 128;
const ITERS = 10;

var D = [1..N, 1..N];
var A: [D] int;

for k in 1..ITERS do
  for i,j in D do
    A(i,j) = i + j + k;

writeln(A(52, 52));
