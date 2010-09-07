def param foo() {
  writeln("In foo() with (nonsensical) param this");
}

def foo() {
  writeln("In foo() with non-param this");
}

foo();
