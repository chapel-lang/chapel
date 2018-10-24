// Double check error message when a non-throws function calls a throwing
// function from a cobegin statement
use ExampleErrors;

module M {
  proc throwingFn() throws {
    throw new StringError("test error");
  }

  proc badFn() {
    cobegin {
      throwingFn();
      throwingFn();
    }
  }
}

use M;
badFn();
