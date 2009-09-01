def foo() {
  var A: [i in 1..3] int = i;
  return A;
}

def bar(z) {
  writeln(z);
}

def main {
  //
  // init copy
  //
  var x = foo();
  writeln(x);

  //
  // assignment
  //
  var y: [1..3] int;
  y = foo();
  writeln(y);

  //
  // auto copy
  //
  bar(foo());
}
