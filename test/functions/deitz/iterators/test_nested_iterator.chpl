proc foo() {
  var n = 4;
  iter bar() {
    for i in 1..n do
      yield i;
  }
  for i in bar() do
    writeln(i);
  n = 8;
  for i in bar() do
    writeln(i);
}

foo();
