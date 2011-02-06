proc param foo() {
  writeln("In foo() with (nonsensical) param this");
}

proc foo() {
  writeln("In foo() with non-param this");
}

foo();
