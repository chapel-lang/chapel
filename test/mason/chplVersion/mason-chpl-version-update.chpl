
use MasonUtils;
use MasonUpdate;

use FileSystem;

config const toml = "";

proc main() {
  const args = ["foo", "update", "--no-update-registry"];

  UpdateLock(args, toml);

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
