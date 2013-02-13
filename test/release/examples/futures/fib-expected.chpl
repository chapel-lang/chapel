use Futures;

proc fib(n: int): int {
  if (n <= 1) {
    return n;
  }
  var n1 = new future(int); begin { n1.put(fib(n - 1)); };
  var n2 = new future(int); begin { n2.put(fib(n - 2)); };
  return n1.get() + n2.get();
}

config const n: int = 8;

proc main(): void {
  var actualFutureVariable = new future(int);
  begin {
    var res = fib(n);
    actualFutureVariable.put(res);
  };
  var res = actualFutureVariable.get();
  writeln("fib(", n, ") = ", res);
}

