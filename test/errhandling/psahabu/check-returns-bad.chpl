proc minusOne(x: int): int throws {
  if x == 0 then
    throw new owned Error();
  return x - 1;
}

proc consumeMinusOne(x: int) {
  try {
    return minusOne(x);
  } catch {
    writeln("caught error");
  }
}

writeln(consumeMinusOne(5));
writeln();
writeln(consumeMinusOne(0));
