module OuterModule {
  proc foo(r: real) { writeln(r); }

  module M {
    proc foo(i: int) { writeln(i); }
  }

  M.foo(3.0);
  foo(3.0);
}
