//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
prototype module PrototypeModule {
  use ErrorHandlingHelper;

  canThrow(-1); // handling can be omitted; halts if an error occurs

  proc throwsErrorsOn() throws {
    // error propagates out of this function
    alwaysThrows();
  }

  proc doesNotThrowErrorsOn() {
    // causes a halt if called
    alwaysThrows();
  }
}

use PrototypeModule;
/* STOP_EXAMPLE_0 */

try {
  throwsErrorsOn();
  halt('Should not reach here!');
} catch e {
  // Good.
}

doesNotThrowErrorsOn();
