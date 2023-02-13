
use IO;

proc main() {
  var f = openMemFile();
  {
    var w = f.writer();
    for 1..2 {
      w.writeLiteral("$");
      w.writeLiteral("$");
      w.writeNewline();
    }
  }

  var r = f.reader();

  r.readLiteral("$");
  r.readLiteral(b"$");
  r.readNewline();

  assert(r.matchLiteral("$"));
  assert(r.matchLiteral(b"$"));
  assert(r.matchNewline());
}
