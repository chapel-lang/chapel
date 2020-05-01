module OuterModule {
  proc foo(r: real) { writeln(r); }

  module M {
    proc foo(i: int) { writeln(i); }
  }

  proc bar() {
    M.foo(3);
  }
  bar();
  foo(3);
}
