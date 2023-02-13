use IO, Regex;

const delimA = compile("\\|{2,}"),
      delimB = compile("\\d{5,}");

readInformation(true, true);
readInformation(true, false);

readInformation(false, true);
readInformation(false, false);

proc readInformation(param reuseBuffer: bool, includes: bool) where reuseBuffer == true {
  var r = openreader("information.txt"),
      s : string;

  assert(r.readUntil(s, delimA, -1, false, includes));
  writeln(s);

  assert(r.readUntil(s, delimB, -1, false, includes));
  writeln(s);
  writeln("--------------------");
}

proc readInformation(param reuseBuffer: bool, includes: bool) where reuseBuffer == false {
  var r = openreader("information.txt");

  var s = r.readUntil(delimA, -1, false, includes);
  writeln(s);

  s = r.readUntil(delimB, -1, false, includes);
  writeln(s);
  writeln("--------------------");
}
