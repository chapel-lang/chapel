proc bar(x) {
  for i in 1..x do
    writeln(i);
}

proc foo(x: int) param {
  bar(x);
  return 7;
}

proc main {
  param n: int = foo(4);
  writeln(n);
}
