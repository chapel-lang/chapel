use IO;

var f = openMemFile();
f.writer(locking=false).write(b"\xff" * 500);
try {
  var s = f.reader(locking=false).readAll(string);
} catch e {
  writeln("caught expected error ", e);
}

