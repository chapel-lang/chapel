// ensure that --optimize-array-indexing does not remove innerMult for programs
// that do strided slicing

var A: [1..10] int;
var B: [1..100] int;

A = B[1..100 by 10];

for i in 1..10 do A[i] = i;
