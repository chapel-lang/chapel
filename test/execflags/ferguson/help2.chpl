use Help;

proc main(args: [] string) {
  var ctr = 1;
  while ctr < args.size {
    if args[ctr]=="--help" {
      printUsage();
    }
    ctr += 1;
  }
}
