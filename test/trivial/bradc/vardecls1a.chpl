var x: int = 2;

proc foo(x: int) {
  var x = 4;
  writeln("then x is: ", x);
}

proc bar() {
  writeln("x in bar() is: ", x);
  var x = 5;
  writeln("then x is: ", x);
}

proc baz() {
  writeln("x in baz() is: ", x);
}

proc main() {
  foo(3);
  writeln();
  bar();
  writeln();
  baz();
  writeln();
}
