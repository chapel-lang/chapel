use IO;

var f = open("foo.txt", ioMode.cw);
try {
  f.check();
} catch e: Error {
  writeln(e.message());
}
f.close();

use FileSystem;
remove("foo.txt"); // So we don't have to clean it up via the testing system
