prototype module OuterModule {
  module A {
    use SysError;

    proc deferThrow() throws {
      defer {
        throw new owned Error();
      }
    }

    proc deferTry() throws {
      defer {
        try {
          throw new owned Error();
        } catch e: UnexpectedEOFError {
          writeln("this shouldn't happen");
        }
      }
    }

    proc deferTryComplete() {
      defer {
        try {
          throw new owned UnexpectedEOFError();
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
