private iter foo(x: int) {
  // Should not be visible outside of definesPrivate
  for i in x..(x+14) {
    yield i;
  }
}

iter bar(y: int) {
  // Should still be visible when module is used.
  for i in 20..(20+y) {
    yield i;
  }
}

proc main() {
  writeln("Running foo");
  for i in foo(2) {
    writeln(i);
  }
  writeln("Running bar");
  for i in bar(2) {
    writeln(i);
  }
}

proc printFoo() {
  for i in foo(2) {
    writeln(i);
  }
}
