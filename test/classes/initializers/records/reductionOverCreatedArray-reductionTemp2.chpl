use BigInteger;

proc reductive_sequential(n:int): bigint {
  var reductTemp = * reduce for i in 1..n do new bigint(i);
  return reductTemp;
}

proc main() {
  writeln(reductive_sequential(0));
}
