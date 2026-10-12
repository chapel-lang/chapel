//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
use ErrorHandlingHelper;

canThrow(-1); // handling can be omitted; halts if an error occurs

proc throwsErrorsOn() throws {
  // error propagates out of this function
  canThrow(1);
}

proc doesNotThrowErrorsOn() {
  // causes a halt if called
  alwaysThrows();
}
/* STOP_EXAMPLE_0 */

try {
  throwsErrorsOn();
  halt('Should not be reached!');
} catch e {
  // Good.
}

// Will halt on this line.
doesNotThrowErrorsOn();
