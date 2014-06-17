var A : [1..5][1..3] real;
var B : [1..3] real;
var C : [1..3] real;
B = + reduce [i in 1..5] A(i);
//for i in 1..5 do
//	B = + reduce A(i);
writeln(B);
