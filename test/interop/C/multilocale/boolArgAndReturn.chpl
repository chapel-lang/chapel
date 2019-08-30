// Chapel file that exports functions: one that takes an bool, one that returns
// an bool, and one that does both.
export proc take(x: bool) {
  writeln("Was given x: ", x);
}

export proc give(): bool {
  return false;
}

export proc takeAndReturn(x: bool): bool {
  return x;
}
