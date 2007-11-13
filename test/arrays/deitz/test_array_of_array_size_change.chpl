var A: [1..3] [1..3] (int, int);

[(i,j) in [1..3, 1..3]] A(i)(j) = (i, j);

writeln(A);

A(2).domain = [1..4];

writeln(A);
