module ThrowContext {

  class ExternCallError: Error { }

  record externCaller: contextManager {
    var retVal: int;

    proc ref enterContext() {
      return this;
    }

    proc ref exitContext(in err: owned Error?) throws {
      if retVal < 0 then
        throw new ExternCallError();
    }
  }

  proc fakeFailingExtern() {
    return -1;
  }

  proc test() throws {
    manage new externCaller() as call { // error handling in defer blocks must be complete
      call.retVal = fakeFailingExtern();
    }
  }

  proc main() {
    try {
      test();
    }
    catch e {
      writeln(e);
    }
  }
}
