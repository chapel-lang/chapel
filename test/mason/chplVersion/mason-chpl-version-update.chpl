
use MasonUtils;
use MasonUpdate;

use FileSystem;

config const toml = "";

extern proc setenv(name : c_string, val : c_string, overwrite : c_int) : c_int;
proc setEnv(name : string, val : string) {
  const ret = setenv(name.c_str(), val.c_str(), 1);
  assert(ret == 0);
}

proc main() {
  const args = ["foo", "update", "--no-update-registry"];
  setEnv("MASON_CACHED_REGISTRY", "./registry");

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
