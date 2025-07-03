//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use ArgumentParser;

/* START_EXAMPLE_0 */
module M {
  use ArgumentParser;

  proc main(args: [] string) throws {
    var parser = new argumentParser();
    var simpleArg = parser.addArgument(name="positional");
    var optionArg = parser.addOption(name="optional");
    var flagArg = parser.addFlag(name="debug", defaultValue=false);
    parser.parseArgs(args);

    var debug = flagArg.valueAsBool();
    // we are assured a value here or else the parser would have thrown an error
    var foo = simpleArg.value();
    var bar: string;
    if optionArg.hasValue() then bar = optionArg.value();
  }
}
/* STOP_EXAMPLE_0 */

{
var parser = new argumentParser();
/* START_EXAMPLE_1 */
// to set the help text on an argument, flag, etc.
var myArg = parser.addArgument(name="myArg",
                               help="This text describes myArg for the user");
/* STOP_EXAMPLE_1 */
}

{
/* START_EXAMPLE_2 */
class MyHelpHandler : HelpHandler {
  override proc printHelp() {
    // call some custom functions like ...
    // printMyCustomHelp();
    // maybe other things?
  }
}
var parser = new argumentParser(helpHandler=new MyHelpHandler());
/* STOP_EXAMPLE_2 */
}

{
/* START_EXAMPLE_3 */
const myHelpMessage = "A customized message that should be displayed\n" +
                      "instead of the usual help  and usage message.\n";
var parser = new argumentParser(helpMessage=myHelpMessage);
/* STOP_EXAMPLE_3 */
}

{
/* START_EXAMPLE_4 */
var parser = new argumentParser();
/* STOP_EXAMPLE_4 */
}
