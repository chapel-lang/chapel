//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use ArgumentParser;

{
var parser = new argumentParser();
/* START_EXAMPLE_0 */
var debugArg = parser.addFlag(name="debug");
/* STOP_EXAMPLE_0 */
}

{
var parser = new argumentParser();
/* START_EXAMPLE_1 */
var debugArg = parser.addFlag(name="debug",
                              numArgs=0..1,
                              defaultValue=false);
/* STOP_EXAMPLE_1 */
}
