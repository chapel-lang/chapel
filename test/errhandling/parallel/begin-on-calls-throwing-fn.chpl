prototype module OuterModule {
  // Double check error message when non-throws function calls throwing function
  // from begin+on statement
  module M {
    use ExampleErrors;
    proc throwingFn() throws {
      throw new owned StringError("test error");
    }

    proc badFn() {
      begin on Locales[numLocales-1] {
        throwingFn();
      }
    }
  }

  use M;
  badFn();
}
