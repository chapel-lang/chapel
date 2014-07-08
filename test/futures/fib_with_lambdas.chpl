use Futures;

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

var n: int = 4;
var r = fib(n);

// print the results
writeln("fib(", n, ") = ", r);