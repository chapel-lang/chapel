var x: integer = 123;

var y: integer;

y = 456;

function foo(z: integer) {
  var a: integer = 789;

  writeln("a is: ", a);
  writeln("z is: ", z);
}

writeln("x is: ", x);
foo(765);
writeln("y is: ", y);
