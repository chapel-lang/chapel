use Types;

proc printSizes(type t) {
  writeln("numBits = ", numBits(t));
  writeln("numBytes = ", numBytes(t));
  writeln("-------------");
}

printSizes(bool);
