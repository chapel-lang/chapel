// ensure that --optimize-array-indexing does not remove innerMult for programs
// that rankChange

var B: [1..10, 1..10] int;
ref A = B[1..10, 1];

for i in 1..10 do A[i] = i;

writeln("B is:\n", B);
writeln("A is: ", A);
