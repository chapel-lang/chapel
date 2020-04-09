use BigInteger;

proc reductive_sequential(n:int): bigint {
  var reductTemp = * reduce for i in 1..n do new bigint(i);
  return reductTemp;
}

proc main() {
  const arr = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 20, 30, 40];
  for i in arr {
    writeln(reductive_sequential(i));
  }
}
