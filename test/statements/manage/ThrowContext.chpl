// This is adapted from Engin's program in #27764.
module ThrowContext {

  class ExternCallError: Error {
    override proc message(): string {
      return 'Extern call error occurred!';
    }
  }

  record externCaller: contextManager {
    var retVal: int;

    proc ref enterContext() ref {
      return this;
    }

    proc exitContext(in err: owned Error?) throws {
      if err then throw err;
      if retVal < 0 then throw new ExternCallError();
    }
  }

  proc fakeFailingExtern() {
    return -1;
  }

  proc test0() throws {
    writeln('Running test...');

    manage new externCaller() as call {
      call.retVal = fakeFailingExtern();
    }

    writeln('Done!');
  }

  // TODO: mention of non-nilable variable after ownership is transferred...?
  proc test1() throws {
    writeln('Running test...');

    manage new externCaller() as call {
      throw new Error('Another unexpected error occurred!');
      call.retVal = fakeFailingExtern();
    }

    writeln('Done!');
  }

  proc main() {
    try { test0(); } catch e { writeln(e); }
    /*
    try { test1(); } catch e { writeln(e); }
    */
  }
}
