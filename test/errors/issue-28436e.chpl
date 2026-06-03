module Foo {

  class AssertionError: Error {  // thanks UnitTest
    proc init(msg: string = "") {
      super.init(msg);
    }
  }

  record RaiseErrorManager: contextManager {
    type errType;

    proc enterContext() {}

    /* note: renamed 'err' to 'error' from original to match interface */
    /* note: removed throws; see https://github.com/chapel-lang/chapel/issues/28148 */
    proc exitContext(in error: owned Error?) {
      try {
        if error then throw error;
        throw new owned AssertionError("manage stmt didn't throw any error!");
      }
      catch e: errType {  // anticipated error
        return;
      }
      catch e {  // unanticipated error
        /* note: removed uncaught throw; see https://github.com/chapel-lang/chapel/issues/28148 */
        try! throw e;
      }
    }
  }

  class SomeError : Error {
    proc init() {}
  }


  proc main() {

    proc foo() throws {
      throw new owned SomeError();
    }

    manage new RaiseErrorManager(owned SomeError) { /* note: added owned to original */
      foo();
    }
  }
}
