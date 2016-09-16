// ensure that --optimize-array-indexing does not remove innerMult for programs
// that use a strided reindex

var A: [1..10] int;
var B: [1..20 by 4] => A[1..10 by 2];

for i in 1..20 by 4 do B[i] = i; 
