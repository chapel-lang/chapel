prototype module OuterModule {
  module A {
    use OS;

    proc deferThrow() throws {
      defer {
        throw new owned Error();
      }
    }

    proc deferTry() throws {
      defer {
        try {
          throw new owned Error();
        } catch e: UnexpectedEofError {
          writeln("this shouldn't happen");
        }
      }
    }

    proc deferTryComplete() {
      defer {
        try {
          throw new owned UnexpectedEofError();
        } catch {
          writeln("this is ok");
        }
      }
    }

    proc deferTryBang() {
      defer {
        try! {
          throw new owned Error();
        }
      }
    }
  }

  A.deferThrow();
  A.deferTry();
  A.deferTryComplete();
  A.deferTryBang();
}
