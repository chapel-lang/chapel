use IO;

param bitsPerHex = 4;

var line: bytes;
readLine(line);
line = line.strip();
writeln(line);

var binary: [0..<bitsPerHex*line.size] uint(8);

// TODO: should be able to iterate in parallel over bytes directly
// see futures/day16-bytesParIter.chpl
forall i in 0..<line.size {
  const ch = line[i];
  const val = charToInt(ch);;
  for param j in 0..<bitsPerHex do
    binary[(i+1)*bitsPerHex-(j+1)] = (val & (0x1 << j)) != 0;
}
writeln(binary);
var cursor: int; // TODO: want local static variable
var versionSum: int;
processCommand(binary);
writeln("VERSION SUM: ", versionSum);

proc charToInt(ch: uint(8)) {
  if ch <= '9'.toByte() then
    return ch - '0'.toByte();
  else
    return ch - 'A'.toByte() + 10;
}

proc processCommand(command: []) {
  const version = binArrToVal(command[cursor..#3]);
  writeln("version = ", version);
  versionSum += version;
  cursor += 3;
  const packetTypeID = binArrToVal(command[cursor..#3]);
  cursor += 3;
  writeln("packet type = ", packetTypeID);
  if packetTypeID == 4 {
    // This is a value
    var len = 0;
    var D = {0..<len};
    var A: [D] uint(8);
    do {
      const last = command[cursor] == 0;
      len += 4;
      D = {0..<len};
      A[..<len#-4] = command[cursor+1..#4];
      cursor += 5;
    } while !last;
    var val = binArrToVal(A);
    writeln("Value = ", val);
  } else {
    // This is an operator
    if command[cursor] == 0 {
      var totBitLength = binArrToVal(command[cursor+1..#15]);
      cursor += 16;
      writeln("totBitLength = ", totBitLength);
      const start = cursor;
      while cursor-start < totBitLength do
        processCommand(command[cursor..]);
    } else {
      var numSubPackets = binArrToVal(command[cursor+1..#11]);
      cursor += 12;
      writeln("numSubPackets = ", numSubPackets);
      for i in 0..<numSubPackets {
        writeln("sub-packet ", i);
        processCommand(command[cursor..]);
      }
    }
  }
}

proc binArrToVal(binArr: [] ) {
  var val = 0;
  for d in binArr {
    val <<= 1;
    val |= d;
  }
  return val;
}
