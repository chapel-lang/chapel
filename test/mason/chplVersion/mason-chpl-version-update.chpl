
private use List;
use MasonUtils;
use MasonUpdate;
use IO;

use FileSystem;

config const toml = "";
var checksum = false;

proc main() {

  updateLock(true, checksum, tf=toml);

  if exists("Mason.lock") {
    writeln("----- lock file -----");

    const fi = open("Mason.lock", iomode.r);
    for line in fi.lines() {
      write(line);
    }
    fi.close();

    remove("Mason.lock");
  }
}
