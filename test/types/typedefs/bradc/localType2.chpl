-- This is a test to make sure that local type default
-- values are evaluated ony when the type is reached
-- and not every time a variable of that type is
-- declared (if, in fact, that is the language's
-- intention.

function readDefaultVal(): int {
  var defaultVal: int;
  read(defaultVal);
  return defaultVal;
}

function foo() {
  typedef myfooint: int = readDefaultVal();
  var x: myfooint;
  var y: myfooint;
  writeln("x is: ", x);
  writeln("y is: ", y);
}

function main() {
  foo();
  foo();
}
