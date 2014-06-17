var InnerD = {1..3};

var A: [1..3] [InnerD] (int, int);

[(i,j) in {1..3, 1..3}] A(i)(j) = (i, j);

writeln(A);

InnerD = {1..4};

writeln(A);
