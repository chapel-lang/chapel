{
  var r: range(int, boundKind.both, false) = 1..3;
  var rngs: 1*range(int, boundKind.both, true);
  // this conditional is a trick to fool split init
  if (true) {
    rngs = (r,);
  }
  writeln(rngs, ": ", rngs.type:string);
}

{
  var r: range(int, boundKind.both, false) = 1..3;
  var rngs: 1*range(int, boundKind.both, true) = (r,);
  writeln(rngs, ": ", rngs.type:string);
}
