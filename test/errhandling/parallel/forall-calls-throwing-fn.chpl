prototype module OuterModule {
  // Test calling a throwing function from a non-throwing function via a forall
  // loop
  use ExampleErrors;

  module M {
    config const n = 1000;
    proc throwingFn() throws { }

    proc test() {
      writeln("before for block");
      forall i in 1..n {
        throwingFn();
      }
      writeln("after for block");
    }
  }

  use M;
  test();
}
