//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
// chpl myprogram.chpl
use Help;

config const myFlag: bool = false;

proc main(args: [] string) {
  var sawHelp = false;

  for a in args {
    if sawHelp {
      writeln("FILE: ", a);

    } else if a == "--help" {
      sawHelp = true;
      printUsage();
      writeln("\nEXTRA ARGUMENTS:");
      writeln(  "================");
      writeln("You can provide an arbitrary list of filenames.");
    }
  }
}
/* STOP_EXAMPLE_0 */
