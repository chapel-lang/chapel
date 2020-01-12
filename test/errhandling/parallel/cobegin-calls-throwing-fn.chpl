prototype module OuterModule {
  // Double check error message when a non-throws function calls a throwing
  // function from a cobegin statement
  use ExampleErrors;
  module M {
    proc throwingFn() throws {
      throw new owned StringError("test error");
    }

    proc badFn() {
      cobegin {
        throwingFn();
        throwingFn();
      }
    }
  }

  use M;
  badFn();
}
