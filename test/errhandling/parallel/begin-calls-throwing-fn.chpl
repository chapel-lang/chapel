prototype module OuterModule {
  // Double check error message when non-throws function calls throwing function
  // from begin statement
  module M {
    use ExampleErrors;
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
