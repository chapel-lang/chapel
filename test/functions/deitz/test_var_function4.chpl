var x: int;

def foo() var {
  writeln(setter);
  return x;
}

def bar() var {
  writeln(setter);
  return foo();
}

bar() = 2;
writeln(bar());
