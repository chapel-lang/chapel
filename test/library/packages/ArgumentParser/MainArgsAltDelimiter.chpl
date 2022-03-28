module M {
  use ArgumentParser;
  config const myConfigConst = "not set";
  proc main(args:[]string) throws {
    var parser = new argumentParser();
    // test using an alternative passthru delimiter which allows more complex
    // mixing of chapel config vars and flags with the argument parser options
    var passThrough = parser.addPassThrough("++");

    var configOpt = parser.addOption(name="myConfigConst");

    var flag = parser.addFlag(name="-q");

    try! {
      parser.parseArgs(args);
    }
    catch ex : ArgumentError {
      writeln(ex.message());
      exit(1);
    }

    writeln("Arguments from command-line:");
    //skip the first value as it can differ between test configs
    for arg in args[1..] {
      writeln(arg);
    }
    writeln("myConfigConst = " + myConfigConst);

    writeln("Parsed Values:");

    writeln("-t " + flag.value());

    writeln("--myConfigConst " + configOpt.value());

    writeln("passed thru vals:");
    for val in passThrough.values() {
      writeln(val);
    }

  }
}
