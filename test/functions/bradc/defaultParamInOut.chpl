
proc callinout2(inout x: int = 100, inout y: int = 200): int {
  x += 1;
  y += 1;
  writeln("in callinout2, x is: ", x, ", y is: ", y);
  writeln("returning: ", x+y);
  return x+y;
}

proc main() {
  var a: int = 10;
  var b: int = 30;
  var r: int;

  r = callinout2();
  writeln("at callsite, a is: ", a, ", b is: ", b, ", r is: ", r);
  writeln();

  r = callinout2(a);
  writeln("at callsite, a is: ", a, ", b is: ", b, ", r is: ", r);
  writeln();

  r = callinout2(a, a);
  writeln("at callsite, a is: ", a, ", b is: ", b, ", r is: ", r);
  writeln();

  r = callinout2(a, b);
  writeln("at callsite, a is: ", a, ", b is: ", b, ", r is: ", r);
  writeln();

  r = callinout2(b, a);
  writeln("at callsite, a is: ", a, ", b is: ", b, ", r is: ", r);
  writeln();
}
