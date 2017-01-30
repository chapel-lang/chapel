proc foo(x: uint) where bar() {
  writeln("In foo");
}

proc bar() param {
  compilerError("Should never get here");
}

foo(1.2);
