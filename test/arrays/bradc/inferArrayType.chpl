config var n = 5;

var D = {1..n, 1..n};
var E = {0..n-1, 0..n-1};

var A: [D] real = [(i,j) in D] (i-1)*n + j;
var B: [E] real = -A;

writeln("A is:\n", A);
writeln("A.domain is: ", A.domain);
writeln();

writeln("B is:\n", B);
writeln("B.domain is: ", B.domain);
writeln();

var X = [i in D] A(i);

writeln("X is:\n", X);
writeln("X.domain is: ", X.domain);
writeln();

var Y = [i in [0..n**2]] B(i/n, i%n);

writeln("Y is:\n", Y);
writeln("Y.domain is: ", Y.domain);
writeln();

var Z = A + B;

writeln("Z is:\n", Z);
writeln("Z.domain is: ", Z.domain);
writeln();

var Z2 = B + A;

writeln("Z2 is:\n", Z2);
writeln("Z2.domain is: ", Z2.domain);
writeln();
