var A: [1..3] [1..3] real;

[i in 1..3] [j in 1..3] A(i)(j) = i + j/10.0;

writeln("A is:\n", A);
