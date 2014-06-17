var D: domain(int);
D += 1;
D += 5;
var A: [1..3] [D] int;
A(1)(1) = 1;
A(1)(5) = 2;
A(2)(1) = 3;
A(3)(5) = 4;
writeln(A);
