def bar(x) {
  for i in 1..x do
    writeln(i);
}

def foo(x: int) param {
  bar(x);
  return 7;
}

def main {
  param n: int = foo(4);
  writeln(n);
}
