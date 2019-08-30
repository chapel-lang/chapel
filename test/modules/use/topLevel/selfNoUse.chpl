module M {
  proc foo() {
    writeln("In foo");
  }

  proc bar() {
    M.foo();
  }

  bar();
}
