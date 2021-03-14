proc foo (x: int, y: int = x) {
  begin writeln (x, ": Hello, world!");
  return y;
}

var z = foo (1);
writeln (z, ": Hello, world!");
