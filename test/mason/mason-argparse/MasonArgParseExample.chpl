module M {
    private use MasonArgParse;
    config var myConfigVar:string;

    proc main(args:[?argsD]string) throws {
      writeln("Config Var Values:");
      writeln(myConfigVar);
      
      writeln("Arguments Received from CL:");
      // print the contents of args, skipping the executable name
      // as it can vary depending on the test environment
      for i in 1..argsD.high {
          writeln(i:string + " " + args[i]);
      }

      var parser = new argumentParser();
    
      var strArg = parser.addOption(name="strArg1",
                                    opts=["-o","--option"],
                                    numArgs=1..10);
      var typArg = parser.addOption(name="strArg2",
                                    opts=["-t","--types"],
                                    numArgs=1..);
      var confArg = parser.addOption(name="strArg3",
                                    opts=["--myConfigVar"],
                                    numArgs=1);

      var subCmd1 = parser.addSubCommand(cmd="subCmd1");

      var rest = parser.parseArgs(args[1..]);
      
      writeln("Values received from argparse:");
      if strArg.hasValue() {         
        for val in strArg.values() do writeln(val);
      }      
      for item in typArg.values() do writeln(item);
      for item in confArg.values() do writeln(item);
      if subCmd1.hasValue() {
        mySubCmd1(rest.toArray());
      }

    }

    proc mySubCmd1(args:[?argsD]string) throws {
      writeln("SubCommand1 was called");
      var parser = new argumentParser();

      var subCmdArg1 = parser.addOption(name="subCmdArg1",
                                        opts=["-c","--cmdArg1"],
                                        numArgs=1);

      var subCmdArg2 = parser.addOption(name="subCmdArg2",
                                        opts=["-t","--cmdArg2"],
                                        numArgs=1);

      var rest = parser.parseArgs(args);
      writeln("args parsed in subcommand:");
      for item in subCmdArg1.values() do writeln(item);
      for item in subCmdArg2.values() do writeln(item);

    }
}

