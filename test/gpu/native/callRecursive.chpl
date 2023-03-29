use GPU;

proc fib(n: int): int {
  if n <= 0 then return 1;
  else return fib(n-1) + fib(n-2);
}

proc main() {
  on here.gpus[0] {
    var A: [0..10] int;
    foreach i in 0..10 {
      assertOnGpu();
      A[i] = fib(i);
    }
    writeln(A);
  }
}
