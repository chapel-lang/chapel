use IO, Regex;

const matchUrl = compile(b"https?:\\/\\/(.*\\.)(org|com)([\\/a-zA-Z]*)?(\\.html)?");

readUrlData(true, true);
readUrlData(true, false);

readUrlData(false, true);
readUrlData(false, false);

proc readUrlData(param reuseBuffer: bool, includes: bool) where reuseBuffer == true {
  var r = openreader("urls.txt"),
      b : bytes;

  while r.readUntil(b, matchUrl, -1, true, includes) {
    writeln(b.strip());
  }
  writeln("--------------------");
}

proc readUrlData(param reuseBuffer: bool, includes: bool) where reuseBuffer == false {
  var r = openreader("urls.txt");

  while true {
    const b = r.readUntil(matchUrl, -1, true, includes);
    if b.isEmpty() then break;
    writeln(b.strip());
  }
  writeln("--------------------");
}
