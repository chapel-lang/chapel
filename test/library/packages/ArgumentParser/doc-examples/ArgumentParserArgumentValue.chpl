//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use ArgumentParser;

var parser = new argumentParser();

var myArg = parser.addOption(name="optional1", numArgs=1);
parser.parseArgs(["progName", "--optional1=foo"]);

/* START_EXAMPLE_0 */
var argVal: string;
if myArg.hasValue() then argVal = myArg.value();
/* STOP_EXAMPLE_0 */

assert(myArg.hasValue());
assert(argVal == "foo");
