use A;

module A {
  proc foo() : int {
    return 10;
  }

  proc defaultFunc(x = foo()) {
    writeln('x = ', x);
  }
}

proc main() {
  defaultFunc();
  writeln('foo = ', foo());
}
