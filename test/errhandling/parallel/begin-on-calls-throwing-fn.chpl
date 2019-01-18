// Double check error message when non-throws function calls throwing function
// from begin+on statement
use ExampleErrors;

module M {
  proc throwingFn() throws {
    throw new StringError("test error");
  }

  proc badFn() {
    begin on Locales[numLocales-1] {
      throwingFn();
    }
  }
}

use M;
badFn();
