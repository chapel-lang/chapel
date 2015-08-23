proc foo(private x: int) {
  writeln("Should never have compiled, private arguments are meaningless");
}

writeln(foo(3));
