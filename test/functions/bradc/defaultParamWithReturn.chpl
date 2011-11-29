
proc callin(in x: int = 100): int {
  writeln("in callin, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
  return x;
}


proc callout(out x: int = 200): int {
  writeln("in callout, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
  return x;
}


proc callinout(inout x: int = 300): int {
  writeln("in callinout, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
  return x;
}


proc callblank(x: int = 400): int {
  writeln("in callblank, x is: ", x);
  var y = x + 1;
  writeln("y assigned to be: ", y);
  return y;
}


proc main() {
  var a: int = 10;
  var b: int;

  b = callin();
  writeln("back at callsite, a is: ", a);
  writeln("back at callsite, b is: ", b);
  writeln();

  b = callout();
  writeln("back at callsite, a is: ", a);
  writeln("back at callsite, b is: ", b);
  writeln();

  b = callinout();
  writeln("back at callsite, a is: ", a);
  writeln("back at callsite, b is: ", b);
  writeln();

  b = callblank();
  writeln("back at callsite, a is: ", a);
  writeln("back at callsite, b is: ", b);
}
