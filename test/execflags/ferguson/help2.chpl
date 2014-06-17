use Help;

proc main(args: [] string) {
  var ctr = 1;
  while ctr < args.numElements {
    if args[ctr]=="--help" {
      printUsage();
    }
    ctr += 1;
  }
}
