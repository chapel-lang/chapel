class R {
  var x: integer;
  var y: float;
}

function foo() {
  var x: integer = 1;
  var y: float = 2.3;

  var r = R(x = x, y = y);

  writeln("r is: ", r);
}

function bar() {
  var x: float = 4.5;
  var y: integer = 6;

  var r = R(x = y, y = x);

  writeln("r is: ", r);
}

function main() {
  foo();
  bar();
}