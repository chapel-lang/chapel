proc foo(x: int, y: int = if x < 0 then -1 else 1) {
  writeln(x, y);
}

foo(2);
foo(-2);
