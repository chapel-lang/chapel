-- This is a test to make sure that local type default
-- values are evaluated each time the type is encountered

function foo(defval: integer) {
  type myfoointeger: integer = defval;
  var x: myfoointeger;
  writeln("x is: ", x);
}

function main() {
  foo(2);
  foo(4);
}
