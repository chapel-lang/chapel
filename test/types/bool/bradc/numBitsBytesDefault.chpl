use Types;

def printSizes(type t) {
  writeln("numBits = ", numBits(t));
  writeln("numBytes = ", numBytes(t));
  writeln("-------------");
}

printSizes(bool);
