
use IO;

proc main() {
  const fname = "tempfile.txt";
  var f = open(fname, iomode.cw);
  {
    var w = f.writer();
    w.write("hello");
  }
  f.close();

  unlink(fname);
}
