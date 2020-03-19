module OuterModule {
  module Bar {
    proc foo(a:int) {
      writeln("foo1");
    }
  }

  proc foo(a:int) {
    writeln("foo2");
  }

  use this.Bar;

  foo(42);
}
