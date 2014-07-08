use Futures;

config const N: int = 4;

proc fib(n: int): int {
  if (n <= 1) {
    return 1;
  }

  // chapel compiler works fine with this, but complains for fib_with_futures.chpl
  var f1 = lambda() { return fib(n - 1); };
  var f2 = lambda() { return fib(n - 2); };

  // wait until futures are resolved
  return f1() + f2();

}

var r = fib(N);

// print the results
writeln("fib(", N, ") = ", r);