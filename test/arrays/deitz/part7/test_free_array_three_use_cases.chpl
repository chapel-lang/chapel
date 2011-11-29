proc foo() {
  var A: [1..3] int = [i in 1..3] i;
  return A;
}

proc bar(z) {
  writeln(z);
}

proc main {
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
