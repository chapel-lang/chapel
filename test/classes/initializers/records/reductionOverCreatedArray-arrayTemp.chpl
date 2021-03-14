use BigInteger;

proc reductive_sequential(n:int): bigint {
  var arrTemp = for i in 1..n do new bigint(i);
  return * reduce arrTemp;
}

proc main() {
  writeln(reductive_sequential(0));
}
