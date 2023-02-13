use IO;

const fileName = "information.txt",
      delimiter = "||||";

// -- if readUntil(myString) -- pattern:
// don't consume the separator, include it in the returned string
testNonConsumingReadUntil(true, true);
// don't consume the separator, don't include it in the returned string
testNonConsumingReadUntil(true, false);

// -- var myString = readUntil(string) -- pattern:
// don't consume the separator, include it in the returned string
testNonConsumingReadUntil(false, true);
// don't consume the separator, don't include it in the returned string
testNonConsumingReadUntil(false, false);

proc testNonConsumingReadUntil(param reuseBuffer: bool, includes) where reuseBuffer == true {
  var r = openreader(fileName),
      info : string;

  assert(r.readUntil(info, delimiter, -1, false, includes));
  writeln(info);

  assert(r.readLine(info, stripNewline=true));
  writeln(info);
}

proc testNonConsumingReadUntil(param reuseBuffer: bool, includes) where reuseBuffer == false {
  var r = openreader(fileName);

  var info = r.readUntil(string, delimiter, -1, false, includes);
  writeln(info);

  info = r.readLine(string, stripNewline=true);
  writeln(info);
}
