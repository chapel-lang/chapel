config const i = 1;
config const j = 1;
config const n = 2;

proc main() {
  var A:[1..n] [1..n] int;

  var sum = 0; 
  for k in 1..n {
    sum += A[i][j] + A[j][i];
    A[i][j] += 1;
    // We shouldn't conclude that A[i][j] doesn't alias A[j][i]
  }
  writeln(sum);

  A[i] = 0;
  A[j] = 0;
  sum = test(A[i], A[j]);
  writeln(sum);
}

proc test(a, b) {
  var sum = 0;
  for k in 1..n {
    sum += a[j] + b[j];
    a[j] += 1;
  }
  return sum;
}
