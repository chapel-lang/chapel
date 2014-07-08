use Futures;

config const N: int = 4;

proc fib(n: int): int {
  if (n <= 1) {
    return 1;
  }

  // Chapel compiler fails to compile this statement, generates an invalid identifier
  // In file included from /tmp/chpl-shamsimam-18982.deleteme/_main.c:31:
  // /tmp/chpl-...deleteme/Futures.c:208:34: error: use of undeclared identifier 'n2'
  // _return_tmp_ = this9(_cast_tmp_, n2, _endCount);

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