proc foo(x: int(?w)) {
  proc bar(x: ?T) {
    return x;
  }

  return bar(x);
}

writeln(foo(42));
