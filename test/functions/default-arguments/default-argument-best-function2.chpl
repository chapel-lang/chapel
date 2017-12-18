module A {
  proc foo() : int {
    return 10;
  }

  proc defaultFunc(x = foo()) {
    writeln('x = ', x);
  }
}

module B {
  proc foo() : int {
    return 20;
  }

  proc main() {
    A.defaultFunc();
  }
}
