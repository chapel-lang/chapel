prototype module outermod {

  proc throwAnotherError() throws {
    throw new Error("test");
  }

  proc testThrows() {
    throwAnotherError();
  }

  module innermod {
    use ThrowError;

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
