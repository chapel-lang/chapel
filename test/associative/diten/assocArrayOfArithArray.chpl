var D: domain(int);
D += 1;
D += 2;
var A: [D] [1..5] real;

[i in D] [j in 1..5] A(i)(j) = i + j/10.0;
writeln(A);

