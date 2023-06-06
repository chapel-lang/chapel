
use IO;

record R {
  var x : int;

  proc readThis(f) {
    writeln("in readThis");
    this.x = f.read(int);
  }
}

proc main() {
  var f = openMemFile();
  f.writer().write(5);
  var r : R;
  try {
    f.reader().read(r);
  } catch e {
    writeln("Error: ", e.message());
  }
  writeln(r);
}
