module OuterModule {
  // Based on #5833
  //
  // At one point we failed to resolve the call to 'bar' because we were looking
  // for it via foo's callsite, which was limited by the 'only foo' clause.

  use B only foo;
  foo('hello');

  module B {
    proc foo(x) {
      bar(x);
    }
    proc bar(x) {
      writeln(x);
    }
  }
}
