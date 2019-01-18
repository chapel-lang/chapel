module A {

  use SysError;

  proc deferThrow() throws {
    defer {
      throw new Error();
    }
  }

  proc deferTry() throws {
    defer {
      try {
        throw new Error();
      } catch e: UnexpectedEOFError {
        writeln("this shouldn't happen");
      }
    }
  }

  proc deferTryComplete() {
    defer {
      try {
        throw new UnexpectedEOFError();
      } catch {
        writeln("this is ok");
      }
    }
  }

  proc deferTryBang() {
    defer {
      try! {
        throw new Error();
      }
    }
  }
}

A.deferThrow();
A.deferTry();
A.deferTryComplete();
A.deferTryBang();
