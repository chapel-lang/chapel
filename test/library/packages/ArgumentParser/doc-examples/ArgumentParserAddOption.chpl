//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use ArgumentParser;

var parser = new argumentParser();

/* START_EXAMPLE_0 */
var fileArg = parser.addOption(name="filename");
/* STOP_EXAMPLE_0 */ 

/* START_EXAMPLE_1 */
var filesArg = parser.addOption(name="filenames", numArgs=1..);
/* STOP_EXAMPLE_1 */ 
