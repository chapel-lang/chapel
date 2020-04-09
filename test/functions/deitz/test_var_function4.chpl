var x: int;

proc foo() ref {
  writeln(true);
  return x;
}
proc foo() {
  writeln(false);
  return x;
}

proc bar() ref {
  writeln(true);
  return foo();
}

proc bar() {
  writeln(false);
  return foo();
}

bar() = 2;
writeln(bar());
