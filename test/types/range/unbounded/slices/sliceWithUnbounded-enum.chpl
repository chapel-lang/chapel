enum color {red, orange, yellow, green, blue, indigo, violet};
use color;

var A: [red..violet] real = [i in 1..7] i;
var B: [red..indigo] real = [i in 1..6] i;

var r = red.. by -2;
var r2 = red.. by -2 align red;
var r3 = red.. by -2 align orange;

writeln(A[r]);
writeln(A[r2]);
writeln(A[r3]);

writeln(B[r]);
writeln(B[r2]);
writeln(B[r3]);

