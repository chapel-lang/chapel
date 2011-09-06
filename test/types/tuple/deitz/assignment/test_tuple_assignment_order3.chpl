var A = [i in 1..4] i;

writeln(A);

(A(1..2), A(3..4)) = (A(3..4), A(1..2));

writeln(A);
