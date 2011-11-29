var x: int = 123;

var y: int;

y = 456;

proc foo(z: int) {
  var a: int = 789;

  writeln("a is: ", a);
  writeln("z is: ", z);
}

writeln("x is: ", x);
foo(765);
writeln("y is: ", y);
