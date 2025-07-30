//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
module ProductionModule {
  use ErrorHandlingHelper;

  // this line would cause a compilation error since the error is not handled
  canThrow(1);

  proc throwsErrorsOn() throws {
    // any error thrown by alwaysThrows will propagate out
    alwaysThrows();
  }

  // this function does not compile because the error is not handled
  proc doesNotThrowErrorsOn() {
    alwaysThrows();
  }
}

use ProductionModule;
/* STOP_EXAMPLE_0 */

throwsErrorsOn();
doesNotThrowErrorsOn();
