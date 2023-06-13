use IO, List;

param bitsPerHex = 4;

config const debug = false;

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
const result = processCommand(binary);
writeln("Final result = ", result);

proc charToInt(ch: uint(8)) {
  if ch <= '9'.toByte() then
    return ch - '0'.toByte();
  else
    return ch - 'A'.toByte() + 10;
}

proc processCommand(command: []): int {
  const version = binArrToVal(command[cursor..#3]);
  writeln("version = ", version);
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
    return val;
  } else {
    // This is an operator
    if command[cursor] == 0 {
      var totBitLength = binArrToVal(command[cursor+1..#15]);
      cursor += 16;
      writeln("totBitLength = ", totBitLength);
      const start = cursor;
      var args: list(int);
      while cursor-start < totBitLength do
        args.pushBack(processCommand(command[cursor..]));
      return processOp(packetTypeID, args);
    } else {
      var numSubPackets = binArrToVal(command[cursor+1..#11]);
      cursor += 12;
      writeln("numSubPackets = ", numSubPackets);
      var subPackets = for i in 0..<numSubPackets do processCommand(command[cursor..]);
      writeln("subPackets: ", subPackets);

      return processOp(packetTypeID, subPackets);
    }
  }
}

proc processOp(op, args) {
  select op {
    when 0 do return + reduce args;
    when 1 do return * reduce args;
    when 2 do return min reduce args;
    when 3 do return max reduce args;
    when 5 do return args[0] > args[1];
    when 6 do return args[0] < args[1];
    when 7 do return args[0] == args[1];
    otherwise
      halt("Unrecognized command");
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
