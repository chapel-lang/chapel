config var n : integer = 8;

var D : domain(1) = 1..n;

var A : [D] integer;

[i in D] A = i;

writeln(A);

var B : [D] integer;

[D] B = A;

writeln(B);
