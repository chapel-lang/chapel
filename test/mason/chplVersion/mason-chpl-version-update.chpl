
private use List;
use MasonUtils;
use MasonUpdate;
use IO;

use FileSystem;

config const toml = "";

proc main() {

  updateLock(true, tf=toml);

  if exists("Mason.lock") {
    writeln("----- lock file -----");

    const fr = openreader("Mason.lock");
    for line in fr.lines() {
      write(line);
    }
    fr.close();

    remove("Mason.lock");
  }
}
