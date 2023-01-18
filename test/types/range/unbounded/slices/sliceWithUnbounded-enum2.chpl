enum color {red, orange, yellow, green, blue, indigo, violet};
use color;

var A: [red..violet] real = [i in 1..7] i;
var B: [red..indigo] real = [i in 1..6] i;

var r = orange.. by 2;
var r2 = orange.. by 2 align yellow;
var r3 = orange.. by -2 align orange;

writeln(A[r]);
writeln(A[r2]);
writeln(A[r3]);

writeln(B[r]);
writeln(B[r2]);
writeln(B[r3]);

