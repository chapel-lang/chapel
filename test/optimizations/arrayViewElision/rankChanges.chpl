var A: [1..10, 1..10] int;
var B: [1..10, 1..10] int = 2;

A[3, 1..5] = B[3, 1..5];

writeln(A);
