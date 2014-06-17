proc foo() {
  var x, y: int;

  x = 1;
  bar();
  y = x;
  writeln(y);

  proc bar() {
    x = 2;
  }
}

foo();
