proc main(args: [] string):int {
  var ctr = 1;
  while ctr < args.numElements {
    writeln(args[ctr]);
    ctr += 1;
  }
  return ctr;
}
