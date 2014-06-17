// This is a test to make sure that local type default
// values are evaluated ony when the type is reached
// and not every time a variable of that type is
// declared (if, in fact, that is the language's
// intention.

proc readDefaultVal(): int {
  var defaultVal: int;
  read(defaultVal);
  return defaultVal;
}

proc foo() {
  type myfooint = int;
  var x: myfooint;
  var y: myfooint;
  writeln("x is: ", x);
  writeln("y is: ", y);
}

proc main() {
  foo();
  foo();
}
