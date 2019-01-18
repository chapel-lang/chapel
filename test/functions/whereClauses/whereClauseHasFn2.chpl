proc bar() param { return false; }

proc foo() where bar() {
  writeln("In foo");
}

proc foo() {
  writeln("In foo2");
}

foo();
