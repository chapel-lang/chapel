use ArgumentParser;
use List;

config var myConfigVar:string;

proc main(args: []string) throws {

  var argsD = args.domain;
  writeln("Config Var Values:");
  writeln(myConfigVar);

  writeln("Arguments Received from CL:");
  // print the contents of args, skipping the executable name
  // as it can vary depending on the test environment
  for i in 1..argsD.high {
      writeln(i:string + " " + args[i]);
  }
  // create a parser for the main entry point
  var parser = new argumentParser(exitOnError=false,
                                  helpHandler=new shared CustomHelpHandler());

  // add a string option that accepts between 1 and 10 values
  var strArg = parser.addOption(name="strArg1",
                                opts=["-o","--option"],
                                numArgs=1..10);
  // add a string option that accepts 1 or more values
  var typArg = parser.addOption(name="strArg2",
                                opts=["-q","--types"],
                                numArgs=1..);
  // add a string option that accepts exactly 1 value
  var confArg = parser.addOption(name="--myConfigVar");
  // add a bool flag that can be turned on (--flagOn) or off (--no-flagOn)
  // but has no value by default
  var boolArg = parser.addFlag(name="flagOn", flagInversion=true);

  // add a positional argument that expects 1 value
  var posArg = parser.addArgument(name="positionalArg");

  // add a subcommand that has its own parser (defined later)
  var subCmd1 = parser.addSubCommand(cmd="subCmd1");

  try {
    // parse the args
    parser.parseArgs(args);
  }
  catch ex : ArgumentError {
    writeln(ex.message());
    exit(1);
  }
  writeln("Values received from argparse:");
  if strArg.hasValue() {
    for val in strArg.values() do writeln(val);
  }
  for item in typArg.values() do writeln(item);
  for item in confArg.values() do writeln(item);
  if boolArg.hasValue() then writeln("boolArg: " + boolArg.value());
  if posArg.hasValue() then writeln("positional value: " + posArg.value());
  if subCmd1.hasValue() {
    mySubCmd1((new list(subCmd1.values())).toArray());
  }


}

proc mySubCmd1(args:[?argsD]string) throws {
  writeln("SubCommand1 was called");
  // create a parser object for this subcommand
  var parser = new argumentParser(exitOnError=false, helpMessage=subCmdHelp());

  // add a subcommand option that takes exactly 1 value
  var subCmdArg1 = parser.addOption(name="subCmdArg1",
                                    opts=["-c","--cmdArg1"],
                                    numArgs=1);

  // add a subcommand option that takes exactly 1 value
  var subCmdArg2 = parser.addOption(name="subCmdArg2",
                                    opts=["-t","--cmdArg2"],
                                    numArgs=1);
  // add a flag that can be set with --subFlagOn or --no-subFlagOn
  // with a default value of false
  var boolArg = parser.addFlag(name="subBoolArg",
                                opts=["-f","--subFlagOn"],
                                defaultValue=false,
                                flagInversion=true);

  // add a positional argument to the subcommand that expects 0 or 1 values
  var subPosArg = parser.addArgument(name="subItem", numArgs=0..1);

  // add a passthrough identifier for collecting remaining args
  // also define a place to read the passed through values
  // the pattern `--` is commonly used for this purpose and is the default
  var passedThrough = parser.addPassThrough();

  // try to parse the arguments, catch exceptions
  try {
    parser.parseArgs(args);
  }
  catch ex : ArgumentError {
    writeln(ex.message());
    exit(1);
  }
  writeln("args parsed in subcommand:");
  for item in subCmdArg1.values() do writeln(item);
  for item in subCmdArg2.values() do writeln(item);
  writeln("subBoolFlag: " + boolArg.value());
  if subPosArg.hasValue() then writeln("subcommand positional value: " +
                                        subPosArg.value());
  for passed in passedThrough.values() do writeln(passed);
}

// define a custom help message writer
proc helpMessage() {
  writeln("Argument Parser Example");
  writeln();
  writeln("Demonstrating some of the ArgumentParser functionality");
  writeln();
  writeln("USAGE:");
  writeln("  ArgumentParserExample <PositionalArg> [options] [[--] [passed values] | [subcommand]]");
  writeln();
  writeln("ARGUMENTS:");
  writeln("  positionalArg  A value specified by position");
  writeln();
  writeln("OPTIONS:");
  writeln("  -o,--option    strArg1 1-10 values");
  writeln("  -q,--types     strArg2 1 or more values");
  writeln("  --[no-]flagon  boolean flag argument");
  writeln();
  writeln("SUBCOMMANDS:");
  writeln("  subCmd1        A subcommand to try");
}

// define a custom help string for the subcommand argument parser help
proc subCmdHelp() {
  var msg: string;
  msg += "Incomplete help for subcommand\n";
  msg += "OPTIONS:\n";
  msg += "  -c,--cmdArg1   subCmdArg1 An optional value\n";
  msg += "  -t,--cmdArg2   subCmdArg2 An optional value";
  return msg;
}

// define a custom help handler to use for main argument parsing help
class CustomHelpHandler : HelpHandler {
  override proc printHelp() {
    helpMessage();
  }
}
