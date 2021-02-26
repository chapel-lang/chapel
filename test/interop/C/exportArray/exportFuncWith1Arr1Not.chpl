export proc foo(ref x: [] int, otherArg: int) {
  writeln(x); // Note: this assumes x will have initial contents
  for i in x.domain {
    x[i] = x[i] + 1;
  }
  writeln("otherArg was ", otherArg);
}
