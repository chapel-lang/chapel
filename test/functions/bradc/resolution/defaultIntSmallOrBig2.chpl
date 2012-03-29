proc plus(x: int(8), y: int(8)) {
  return (x + y): int(8);
}

proc plus(x: int(16), y: int(16)) {
  return (x + y): int(16);
}

writeln("3*127 = ", plus(127, plus(127, 127)));
