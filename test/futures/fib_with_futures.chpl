use Futures;

proc fib(n: int): int {
  if (n <= 1) {
    return 1;
  }

  // Chapel compiler fails to compile this statement, generates an invalid identifier
  // In file included from /tmp/chpl-shamsimam-18982.deleteme/_main.c:31:
  // /tmp/chpl-...deleteme/Futures.c:208:34: error: use of undeclared identifier 'n2'
  // _return_tmp_ = this9(_cast_tmp_, n2, _endCount);

  var f1 = buildFuture(int, lambda() { return fib(n - 1); });
  var f2 = buildFuture(int, lambda() { return fib(n - 2); });

  // wait until futures are resolved
  return f1.read() + f2.read();

}

var n: int = 4;
var r = fib(n);

// print the results
writeln("fib(", n, ") = ", r);