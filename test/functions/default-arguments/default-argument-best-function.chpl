use A;

module A {
  proc foo() : int {
    return 10;
  }

  proc defaultFunc(x = foo()) {
    writeln('x = ', x);
  }
}

proc foo() : int {
  return 20;
}

proc main() {
  defaultFunc();
}
