var A: [1..10] int;
var B: [1..20] int;

// non-strided slicing does not prevent optimization
A = B[11..20];

// reindexing when lhs is not strided does not prevent optimization
var C => A[2..9];
//var D => A[1..10 by 2];  // commented out -- TODO EJR
var E: [0..9] => A[1..10];

for i in 1..10 do A[i] = i;
