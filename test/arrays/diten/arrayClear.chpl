var A: [1..10] int = [i in 1..10] i;

writeln(A);
A.clear();
writeln((A, A.size));
