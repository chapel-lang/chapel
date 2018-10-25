// Double check error message when a non-throws function calls a throwing
// function from a cobegin+on statement
use ExampleErrors;

module M {
  proc throwingFn() throws {
    throw new StringError("test error");
  }

  proc badFn() {
    cobegin {
      {
        on Locales[numLocales-1] {
          throwingFn();
        }
      }
      {
        on Locales[numLocales-1] {
          throwingFn();
        }
      }
    }
  }
}

use M;
badFn();
