{
  var r: range(int, stridable=false) = 1..3;
  var rngs: 1*range(int, stridable=true);
  // this conditional is a trick to fool split init
  if (true) {
    rngs = (r,);
  }
  writeln(rngs, ": ", rngs.type:string);
}

{
  var r: range(int, stridable=false) = 1..3;
  var rngs: 1*range(int, stridable=true) = (r,);
  writeln(rngs, ": ", rngs.type:string);
}
