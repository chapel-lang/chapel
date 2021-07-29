module M {
    private use MasonArgParse;
    config var myConfigVar:string;

    proc main(args:[?argsD]string) throws{
        writeln("Config Var Values:");
        writeln(myConfigVar);
        
        writeln("Arguments Received from CL:");
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
        parser.parseArgs(args[1..]);
        
        writeln("Values received from argparse:");
        if strArg.hasValue() {         
          for val in strArg.values() do writeln(val);
        }      
        for item in typArg.values() do writeln(item);
        for item in confArg.values() do writeln(item);

    }
}

