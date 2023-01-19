var A: [1..10] real = [i in 1..10] i;
var B: [1..9] real = [i in 1..9] i;

var r = 1.. by -2;
var r2 = 1.. by -2 align 1;
var r3 = 1.. by -2 align 2;

writeln(A[r]);
writeln(A[r2]);
writeln(A[r3]);

writeln(B[r]);
writeln(B[r2]);
writeln(B[r3]);

