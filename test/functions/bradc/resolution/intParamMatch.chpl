proc foo(x: int(64)) {
  writeln("In 64-bit version");
}

proc foo(x: int(32)) {
  writeln("In 32-bit version");
}

foo(1);


proc plus(x: int(64), y: int(64)) {
  writeln("In 64-bit version");
  return x + y;
}

proc plus(x: int(8), y: int(8)) {
  writeln("In 8-bit version");
  return (x+y): int(8);
}

writeln("128 + 128 is: ", plus(128, 128));
