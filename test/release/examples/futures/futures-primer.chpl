use Futures;

proc helper(x: int): int {
  writeln("helper(", x, ")");
  return x;
}

proc main(): void {
  // either the begin block or the future variable must be annotated by type
  var actualFutureVariable : future(int) = begin {
    helper(1);
    helper(2);
    helper(3);
  };
  var res = actualFutureVariable.get();
  assert(res == 3);
  writeln("actualFutureVariable.get() = ", res);
}
