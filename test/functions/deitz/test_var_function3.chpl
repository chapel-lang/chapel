var x: int;

def foo() var {
  writeln(setter);
  return x;
}

foo() = 2;
writeln(foo());
