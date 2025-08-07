//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use JSON;
use IO;

/* START_EXAMPLE_0 */
class Parent {
  var x : int;
}

class Child: Parent {
  var y : int;
}
/* STOP_EXAMPLE_0 */

var c = new Child(x=5, y=2);
stdout.withSerializer(jsonSerializer).writeln(c);
