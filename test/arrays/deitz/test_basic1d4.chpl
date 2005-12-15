config var n : integer = 8;

var D : domain(1) = [1..n];

var A : [D] integer;

[i in D] A(i) = i;

writeln(A);

var B : [D] integer;

[i in D] B(i) = A(i) + 1;

writeln(B);
