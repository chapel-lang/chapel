var x: int;

proc foo() ref {
  writeln(setter);
  return x;
}

foo() = 2;
writeln(foo());
