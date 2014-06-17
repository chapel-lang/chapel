proc plus(x: int(8), y: int(8)) {
  return (x + y): int(8);
}

proc plus(x: int(64), y: int(64)) {
  return (x + y): int(64);
}

writeln("3*127 = ", plus(127, plus(127, 127)));
