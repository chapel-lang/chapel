proc foo(param x: bool, param y: bool) where x && y {
  writeln("x and y are true!");
}

proc foo(param x: bool, param y: bool) where x && !y {
  writeln("x is true and y is false!");
}

proc foo(param x: bool, param y: bool) where !x && y {
  writeln("y is true and x is false!");
}

proc foo(param x: bool, param y: bool) {
  writeln("x and y are both false!");
}

foo(true, true);
foo(true, false);
foo(false, true);
foo(false, false);
