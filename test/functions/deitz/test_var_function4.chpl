var x: int;

proc foo() ref {
  writeln(setter);
  return x;
}

proc bar() ref {
  writeln(setter);
  return foo();
}

bar() = 2;
writeln(bar());
