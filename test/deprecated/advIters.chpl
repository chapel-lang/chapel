use AdvancedIters;

var A: [1..10] int;

for i in dynamic(1..10, chunkSize = 1) do
  A[i] = i;

writeln("A is: ", A);
 