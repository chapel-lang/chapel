module OuterModule {
  proc foo(r: real) { writeln(r); }

  module M {
    proc foo(i: int) { writeln(i); }
  }

  {
    use M;
    M.foo(3);
  }
  foo(3);
}
