use Types;

proc printSizes(type t) {
  writeln("numBits = ", numBits(t));
  writeln("numBytes = ", numBytes(t));
  writeln("-------------");
}

printSizes(bool(8));
printSizes(bool(16));
printSizes(bool(32));
printSizes(bool(64));
