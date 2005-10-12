function foo(x : integer = 2, y : integer = x * x) : integer {
  return x + y;
}

writeln(foo(4));
