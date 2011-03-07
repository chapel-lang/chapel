var x: int;

proc foo() var {
  writeln(setter);
  return x;
}

foo() = 2;
writeln(foo());
