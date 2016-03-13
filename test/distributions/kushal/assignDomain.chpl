use CyclicDist;

config const n=3;   //Indicating the range of the domain

var A = {1..n} dmapped Cyclic(startIdx=1);
var B = {1..n};

var arrA : [A] int;
var arrB : [B] int;

writeln("arrA is: ", arrA);
writeln("arrB is: ", arrB);
writeln();

A = B;
writeln("arrA is: ", arrA);
writeln("arrB is: ", arrB);
writeln();

B = A;
writeln("arrA is: ", arrA);
writeln("arrB is: ", arrB);
