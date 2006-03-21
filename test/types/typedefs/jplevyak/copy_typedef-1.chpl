-- This is a test to make sure that local type default
-- values are evaluated each time the type is encountered

function foo(param defval: int) {
  typedef myfooint: int = defval;
  var x: myfooint;
  writeln("x is: ", x);
}

function main() {
  foo(2);
  foo(4);
}
