prototype module OuterModule {
  // Double check error message when non-throws function calls throwing function
  // from begin statement
  use ExampleErrors;
  module M {
    proc throwingFn() throws {
      throw new owned StringError("test error");
    }

    proc badFn() {
      begin {
        throwingFn();
      }
    }
  }

  use M;
  badFn();
}
