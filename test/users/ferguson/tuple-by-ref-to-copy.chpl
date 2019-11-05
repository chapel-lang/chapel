proc chan_red(ref args ...?k)
{
  var localArgs:args.type;
  localArgs[0] = 17;
  args[0] = localArgs[0];
}

var a=0;
chan_red(a);
writeln(a);
