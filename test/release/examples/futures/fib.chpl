use Futures;

proc fib(n: int): int {
  if (n <= 1) {
    return n;
  }
  var n1 = begin : int { fib(n - 1); };;
  var n2 = begin : int { fib(n - 2); };;
  return n1.get() + n2.get();
}

config const n: int = 8;

proc main(): void {
  var actualFutureVariable = begin : int { fib(n); };;
  var res = actualFutureVariable.get();
  writeln("fib(", n, ") = ", res);
}
