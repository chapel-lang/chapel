module CanAlwaysThrowWithinManage {
  use TrackingRecord;

  proc r.enterContext() {}
  proc r.exitContext(in e: owned Error?) throws {
    if e then throw e;
  }

  proc alwaysThrows() throws {
    throw new Error('It halts at the \'manage\'!');
  }

  proc codeThatContainsManage() {
    manage new r() {
      writeln('What happens if I throw?!');
      alwaysThrows();
    }
  }

  proc main() {
    codeThatContainsManage();
  }
}
