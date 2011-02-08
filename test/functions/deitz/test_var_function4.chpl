var x: int;

proc foo() var {
  writeln(setter);
  return x;
}

proc bar() var {
  writeln(setter);
  return foo();
}

bar() = 2;
writeln(bar());
