// Chapel file that exports functions: one that takes multiple args and returns
// nothing, one that takes multiple args of different types, and one that takes
// multiple args and returns something
export proc take2(x: int, y: int) {
  writeln("Was given x: ", x, ", and y: ", y);
}

export proc take2Diff(x: int, in y: real) {
  writeln("real is: ", y);
  writeln("int is: ", x);
}

export proc take2AndReturn(x: int, y: int): int {
  writeln("x + y = ", (x + y));
  return x + y;
}
