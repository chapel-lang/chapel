//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
pragma "error mode strict"
module StrictModule {
  use ErrorHandlingHelper;

  proc throwsErrorsOn() throws {
    try {
      alwaysThrows();
      writeln("never reached");
    }
  }

  proc alsoThrowsErrorsOn() throws {
    try alwaysThrows();
  }

  proc doesNotThrowErrorsOn() {
    try! alwaysThrows();
  }

  proc assignmentTry() throws {
    var x = try alwaysThrows();
    writeln(x);
  }
}
/* STOP_EXAMPLE_0 */

use StrictModule;

try {
  throwsErrorsOn();
  halt('Should not reach!');
} catch e {
  // Good.
}

try {
  alsoThrowsErrorsOn();
  halt('Should not reach!');
} catch e {
  // Good.
}

try {
  assignmentTry();
  halt('Should not reach!');
} catch e {
  // Good.
}

doesNotThrowErrorsOn();
