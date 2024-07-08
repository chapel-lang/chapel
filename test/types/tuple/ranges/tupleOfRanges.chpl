{
  var r: range(int, strides=strideKind.one) = 1..3;
  var rngs: 1*range(int, strides=strideKind.any);
  // this conditional is a trick to fool split init
  if (true) {
    rngs = (r,);
  }
  writeln(rngs, ": ", rngs.type:string);
}

{
  var r: range(int, strides=strideKind.one) = 1..3;
  var rngs: 1*range(int, strides=strideKind.any) = (r,);
  writeln(rngs, ": ", rngs.type:string);
}
