proc chan_red(ref args ...?k)
{
  var localArgs:args.type;
  localArgs[1] = 17;
  args[1] = localArgs[1];
}

var a=0;
chan_red(a);
writeln(a);
