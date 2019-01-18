use BigInteger;

proc reductive_sequential(n:int): bigint {
  return * reduce for i in 1..n do new bigint(i);
}

proc main() {
  writeln(reductive_sequential(0)); // BINGO
}
