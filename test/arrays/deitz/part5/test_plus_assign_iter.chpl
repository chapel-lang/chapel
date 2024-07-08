var A, B, C: [1..3] int = 1..3;
// B(C(1..3)) += A;
forall (a,c) in zip(A, C(1..3)) with (ref B) do B[c] += a;
writeln(B);
