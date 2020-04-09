var x: int;

proc foo() ref {
  writeln(true);
  return x;
}
proc foo() {
  writeln(false);
  return x;
}

foo() = 2;
writeln(foo());
