//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
/* START_EXAMPLE */
use List, Help;

config const x = 5;

proc main(args: [] string) {
  var filenames: list(string);

  var programName = args[0];

  for a in args[1..] {
    if a == "-h" || a == "--help" {
      writeln("Usage: ", programName, " <options> filename [filenames]");
      printUsage();
      exit(1); // returning 1 from main is also an option
    } else {
      filenames.pushBack(a);
    }
  }

  writef("Got filenames = %?\n", filenames);
  writef("Got configuration variable x = %?\n", x);
}
/* STOP_EXAMPLE */
