-- This is a test to make sure that local type default
-- values are evaluated ony when the type is reached
-- and not every time a variable of that type is
-- declared (if, in fact, that is the language's
-- intention.

function readDefaultVal(): integer {
  var defaultVal: integer;
  read(defaultVal);
  return defaultVal;
}

function foo() {
  type myfoointeger: integer = readDefaultVal();
  var x: myfoointeger;
  var y: myfoointeger;
  writeln("x is: ", x);
  writeln("y is: ", y);
}

function main() {
  foo();
  foo();
}
