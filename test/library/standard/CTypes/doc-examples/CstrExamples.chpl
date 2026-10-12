//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use CTypes;
use IO;

{
var differentLocale = here; // a lie for the test

/* START_EXAMPLE_0 */
var myString = "Hello!";
on differentLocale {
  stdout.writeBinary(myString.localize().c_str(), myString.numBytes);
}
/* STOP_EXAMPLE_0 */
writeln();
}

{
var differentLocale = here; // a lie for the test

/* START_EXAMPLE_1 */
var myBytes = b"Hello!";
on differentLocale {
  stdout.writeBinary(myBytes.localize().c_str(), myBytes.numBytes);
}
/* STOP_EXAMPLE_1 */
writeln();
}
