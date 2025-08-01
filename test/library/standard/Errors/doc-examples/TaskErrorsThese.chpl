//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use Errors;

inline proc beginTasksThatThrowErrors() {
  var ret = new owned TaskErrors();

  try {
    forall i in 0..0 do throw new Error('Error in task!');
  } catch e : TaskErrors {
    ret = e;
  } catch e {
    writeln('Unexpected!');
  }

  return ret;
}

/* START_EXAMPLE_0 */
var taskErrors: TaskErrors = beginTasksThatThrowErrors();
for containedError in taskErrors {
 // Do something with the contained error
}
/* STOP_EXAMPLE_0 */

var num = 0;
for err in taskErrors do num += 1;
assert(num == 1);
