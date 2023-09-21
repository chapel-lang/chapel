var A: [1..3] [1..3] real;

[i in 1..3 with (ref A)] [j in 1..3 with (ref A)] A(i)(j) = i + j/10.0;

writeln("A is:\n", A);
