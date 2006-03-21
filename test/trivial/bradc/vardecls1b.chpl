var x: int = 2;

function foo(x: int) {
  var x = 4;
  writeln("then x is: ", x);
}

function bar() {
  var x = 5;
  writeln("then x is: ", x);
}

function baz() {
  writeln("x in baz() is: ", x);
}

function main() {
  foo(3);
  writeln();
  bar();
  writeln();
  baz();
  writeln();
}
