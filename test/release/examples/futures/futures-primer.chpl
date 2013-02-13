use Futures;

proc helper(x: int): int {
  writeln("helper(", x, ")");
  return x;
}

proc main(): void {
  var actualFutureVariable = begin : int {
    helper(1);
    helper(2);
    helper(3);
  };;
  var res = actualFutureVariable.get();
  assert(res == 3);
  writeln("actualFutureVariable.get() = ", res);
}
