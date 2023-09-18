var A, B, C: [1..3] int;

C = (1, 3, 2);
B = (2, 3, 1);
// A(B(C)) = 1..3;
forall (b,i) in zip(B(C), 1..3) with (ref A) do A[b] = i;
writeln(C);
writeln(B);
writeln(A);
