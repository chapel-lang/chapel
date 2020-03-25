module OuterModule {
  module NNNNN {
    proc foo() {
      writeln("In sub N.foo");
    }
  }

  use NNNNN;
  foo();
}
