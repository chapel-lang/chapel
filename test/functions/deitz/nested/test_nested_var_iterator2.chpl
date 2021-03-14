proc foo() {
  var x, y: int;

  for i in bar() {
    x = 1;
    i = 2;
    y = x;
  }
  writeln(y);

  iter bar() ref {
    yield x;
  }
}

foo();
