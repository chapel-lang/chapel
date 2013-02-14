use Futures;

proc fib(n: int): int {
  if (n <= 1) {
    return n;
  }
  // either the begin block or the future variable must be annotated by type
  var n1 = begin : int { fib(n - 1); };
  var n2: future(int) = begin { fib(n - 2); };
  return n1.get() + n2.get();
}

config const n: int = 8;

proc main(): void {
  // either the begin block or the future variable must be annotated by type
  var actualFutureVariable: future(int) = begin { fib(n); };
  var res = actualFutureVariable.get();
  writeln("fib(", n, ") = ", res);
}
