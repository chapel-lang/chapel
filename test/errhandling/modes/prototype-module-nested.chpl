prototype module outermod {

  proc throwAnotherError() throws {
    use ExampleErrors;
    throw new StringError("test");
  }

  proc testThrows() {
    throwAnotherError();
  }

  module innermod {
    use ExampleErrors;

    proc propError() {
      throwAnError();
    }

    proc main {
      try {
        testThrows();
        propError();
      } catch {
        writeln("in catch");
      }
    }
  }
}
