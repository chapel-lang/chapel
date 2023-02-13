use IO, Regex;

const matchUrl = compile("https?:\\/\\/(.*\\.)(org|com)([\\/a-zA-Z]*)?(\\.html)?");

readUrlData(true, true);
readUrlData(true, false);

readUrlData(false, true);
readUrlData(false, false);

proc readUrlData(param reuseBuffer: bool, includes: bool) where reuseBuffer == true {
  var r = openreader("urls.txt"),
      s : string;

  while r.readUntil(s, matchUrl, -1, true, includes) {
    writeln(s.strip());
  }
  writeln("--------------------");
}

proc readUrlData(param reuseBuffer: bool, includes: bool) where reuseBuffer == false {
  var r = openreader("urls.txt");

  while true {
    const s = r.readUntil(matchUrl, -1, true, includes);
    if s.isEmpty() then break;
    writeln(s.strip());
  }
  writeln("--------------------");
}
