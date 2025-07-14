//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use ArgumentParser;
use List;

var parser = new argumentParser();

var myArg = parser.addOption(name="optional1", numArgs=1..);
parser.parseArgs(["progName", "--optional1=foo", "bar", "baz"]);

/* START_EXAMPLE_0 */
// to get an array of strings
var argValues = myArg.values();
// create a list of strings
var argList = new list(myArg.values());
/* STOP_EXAMPLE_0 */

assert(argValues.size == 3);
assert(argList.size == argValues.size);
assert(argList[0] == "foo");
assert(argList[1] == "bar");
assert(argList[2] == "baz");
