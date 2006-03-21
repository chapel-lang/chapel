-- This is a test to make sure that local type default
-- values are evaluated ony when the type is reached
-- and not every time a variable of that type is
-- declared (if, in fact, that is the language's
-- intention.

fun readDefaultVal(): int {
  var defaultVal: int;
  read(defaultVal);
  return defaultVal;
}

fun foo() {
  typedef myfooint: int = readDefaultVal();
  var x: myfooint;
  var y: myfooint;
  writeln("x is: ", x);
  writeln("y is: ", y);
}

fun main() {
  foo();
  foo();
}
