var A, B: [1..4] int;
var s$: sync bool;

proc f(i)
  return i;

begin {
  for i in 1..4 {
    A[i] = i;
    s$ = true;
  }
}

for i in 1..4 {
  s$;
  B[i] = A[i] * A[i];
  B[i] += A[i] + f(A[i]);
}

writeln(A);
writeln(B);
