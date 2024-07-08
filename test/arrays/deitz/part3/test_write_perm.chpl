var A, B: [1..3] int;
B = (3, 1, 2);
// A(B) = 1..3;
forall (b,i) in zip(B, 1..3) with (ref A) do A[b] = i;
writeln(A);
writeln(B);
