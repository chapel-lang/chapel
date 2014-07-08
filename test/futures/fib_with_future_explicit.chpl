use Futures;

config const N: int = 4;

proc fib(n: int): int {
  if (n <= 1) {
    return 1;
  }

  var f1 = new future(int);
  begin { f1.write(fib(n - 1)); }

  var f2 = new future(int);
  begin { f2.write(fib(n - 2)); }

  // wait until futures are resolved
  return f1.read() + f2.read();

}

var r = fib(N);

// print the results
writeln("fib(", N, ") = ", r);