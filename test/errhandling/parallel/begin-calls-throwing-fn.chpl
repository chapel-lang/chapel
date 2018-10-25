// Double check error message when non-throws function calls throwing function
// from begin statement
use ExampleErrors;

module M {
  proc throwingFn() throws {
    throw new StringError("test error");
  }

  proc badFn() {
    begin {
      throwingFn();
    }
  }
}

use M;
badFn();
