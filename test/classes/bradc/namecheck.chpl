class R {
  var x: int;
  var y: real;
}

proc foo() {
  var x: int = 1;
  var y: real = 2.3;

  var r = new R(x = x, y = y);

  writeln("r is: ", r);
}

proc bar() {
  var x: real = 4.5;
  var y: int = 6;

  var r = new R(x = y, y = x);

  writeln("r is: ", r);
}

proc main() {
  foo();
  bar();
}
