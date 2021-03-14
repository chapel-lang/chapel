proc bar() param { return true; }

proc foo() where bar() {
  writeln("In foo");
}

proc foo() {
  writeln("In foo2");
}

foo();
