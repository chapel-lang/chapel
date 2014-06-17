proc main(args: [] string) {
  var ctr = 1;
  while ctr < args.numElements {
    writeln(args[ctr]);
    ctr += 1;
  }
}
