use IO;

const fileName = "information.txt",
      delimiterA = b"||||",
      delimiterB = b"1234";

// -- if readUntil(myBytes) -- pattern:
// don't consume the separator, include it in the returned bytes
testNonConsumingReadUntil(true, true);
// don't consume the separator, don't include it in the returned bytes
testNonConsumingReadUntil(true, false);

// -- var myBytes = readUntil(bytes) -- pattern:
// don't consume the separator, include it in the returned bytes
testNonConsumingReadUntil(false, true);
// don't consume the separator, don't include it in the returned bytes
testNonConsumingReadUntil(false, false);

proc testNonConsumingReadUntil(param reuseBuffer: bool, includes) where reuseBuffer == true {
  var r = openreader(fileName),
      info : bytes;

  assert(r.readUntil(info, delimiterA, -1, false, includes));
  writeln(info);

  assert(r.readUntil(info, delimiterB, -1, false, includes));
  writeln(info);
}

proc testNonConsumingReadUntil(param reuseBuffer: bool, includes) where reuseBuffer == false {
  var r = openreader(fileName);

  var info = r.readUntil(bytes, delimiterA, -1, false, includes);
  writeln(info);

  info = r.readUntil(bytes, delimiterB, -1, false, includes);
  writeln(info);
}
