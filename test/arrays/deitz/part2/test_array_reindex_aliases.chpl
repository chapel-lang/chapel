config const k: int = 3, n: int = 10;

proc foo(A: [1..k]) {
  writeln(A);
}

proc main {
  var A: [1..n] int = [i in 1..n] i;

  for i in 1..n-k+1 do
    foo(A[i..i+k-1].reindex({1..k}));
}
