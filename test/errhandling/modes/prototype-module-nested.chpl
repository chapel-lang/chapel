prototype module outermod {

  proc throwAnotherError() throws {
    use ExampleErrors;
    throw new owned StringError("test");
  }

  proc testThrows() {
    throwAnotherError();
  }

  module innermod {
    use ExampleErrors, outermod;

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
