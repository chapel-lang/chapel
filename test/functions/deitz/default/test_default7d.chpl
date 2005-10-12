function foo(x : integer = 2, y : integer = x * x) : integer {
  return x + y;
}

writeln(foo(y=2,x=2));
