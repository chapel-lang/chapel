var A: [false..true] real = [i in 0..1] i;
var B: [false..false] real = [i in 0..0] i;

var r = false.. by 2;
var r2 = false.. by 2 align false;
var r3 = false.. by 2 align true;

writeln(A[r]);
writeln(A[r2]);
writeln(A[r3]);

writeln(B[r]);
writeln(B[r2]);
writeln(B[r3]);

