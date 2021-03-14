use BlockDist;
config const n = 5;
const D2 = {1..n, 1..n} dmapped Block({1..n, 1..n});
const D4 = {0..n+1, 0..n+1, 0..n+1, 0..n+1} dmapped Block({1..n, 1..n, 1..n, 1..n});
var A: [D4] 4*int = D4;
var B: [D2] 4*int;

B = A[1..n, 1, n, 1..n];
writeln(B);

