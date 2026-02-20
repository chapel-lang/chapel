
private use List;
use MasonUtils;
use MasonUpdate;
use IO;

use FileSystem;

config const toml = "";

proc main() {
  try! updateLock(true, tf=toml);

  if exists("Mason.lock") {
    writeln("----- lock file -----");

    const fr = openReader("Mason.lock", locking=false);
    for line in fr.lines() {
      write(line);
    }
    fr.close();

    remove("Mason.lock");
  }
}
