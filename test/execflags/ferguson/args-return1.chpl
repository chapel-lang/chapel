proc main(args: [] string) {
  var ctr = 1;
  while ctr < args.size {
    writeln(args[ctr]);
    ctr += 1;
  }
  return ctr;
}
