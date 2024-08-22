use IO;
use CTypes;
import OS.POSIX.EILSEQ;

var f = openMemFile();
f.writer(locking=false).write(b"\xff" * 500);
try {
  var s = f.reader(locking=false).readAll(string);
} catch e:SystemError {
  assert(e.err == EILSEQ);
  writeln("caught expected error");
} catch {
  writeln("caught unexpected error");
}

