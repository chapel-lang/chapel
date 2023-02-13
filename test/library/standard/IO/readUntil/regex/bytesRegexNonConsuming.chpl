use IO, Regex;

const delimA = compile(b"\\|{2,}"),
      delimB = compile(b"\\d{5,}");

readInformation(true, true);
readInformation(true, false);

readInformation(false, true);
readInformation(false, false);

proc readInformation(param reuseBuffer: bool, includes: bool) where reuseBuffer == true {
  var r = openreader("information.txt"),
      b : bytes;

  assert(r.readUntil(b, delimA, -1, false, includes));
  writeln(b);

  assert(r.readUntil(b, delimB, -1, false, includes));
  writeln(b);
  writeln("--------------------");
}

proc readInformation(param reuseBuffer: bool, includes: bool) where reuseBuffer == false {
  var r = openreader("information.txt");

  var b = r.readUntil(delimA, -1, false, includes);
  writeln(b);

  b = r.readUntil(delimB, -1, false, includes);
  writeln(b);
  writeln("--------------------");
}
