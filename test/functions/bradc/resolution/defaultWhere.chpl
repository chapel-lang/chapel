def foo(param x: bool, param y: bool) where x && y {
  writeln("x and y are true!");
}

def foo(param x: bool, param y: bool) where x && !y {
  writeln("x is true and y is false!");
}

def foo(param x: bool, param y: bool) where !x && y {
  writeln("y is true and x is false!");
}

def foo(param x: bool, param y: bool) {
  writeln("x and y are both false!");
}

foo(true, true);
foo(true, false);
foo(false, true);
foo(false, false);
