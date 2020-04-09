// Chapel file that exports functions: one that takes an int, one that returns
// an int, and one that does both.
export proc take(x: int) {
  writeln("Was given x: ", x);
}

export proc give(): int {
  return 5;
}

export proc takeAndReturn(x: int): int {
  return x;
}
