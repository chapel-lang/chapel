
use IO;

record R {
  var x : int;

  proc ref readThis(f) {
    writeln("in readThis");
    this.x = f.read(int);
  }
}

proc main() {
  var f = openMemFile();
  f.writer(locking=false).write(5);
  var r : R;
  try {
    f.reader(locking=false).read(r);
  } catch e {
    writeln("Error: ", e.message());
  }
  writeln(r);
}
