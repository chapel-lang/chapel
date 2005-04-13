var x: integer = 2;

function foo(x: integer) {
  var x = 4;
  writeln("then x is: ", x);
}

function bar() {
  writeln("x in bar() is: ", x);
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
