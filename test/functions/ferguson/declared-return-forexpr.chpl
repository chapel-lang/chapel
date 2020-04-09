config const n = 10;

proc testproc(): [1..n] int {
  return for i in 1..n do i;
}

var A = testproc();

writeln(A);

