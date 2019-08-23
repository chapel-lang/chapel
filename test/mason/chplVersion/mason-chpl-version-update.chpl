
private use List;
use MasonUtils;
use MasonUpdate;

use FileSystem;

config const toml = "";

proc main() {
  var args: list(string);
  args.append("foo");
  args.append("update");
  args.append("--no-update");

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
