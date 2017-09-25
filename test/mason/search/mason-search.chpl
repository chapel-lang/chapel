
use MasonSearch;

use FileSystem;

config const pattern = "";

extern proc setenv(name : c_string, val : c_string, overwrite : c_int) : c_int;
proc setEnv(name : string, val : string) {
  const ret = setenv(name.c_str(), val.c_str(), 1);
  assert(ret == 0);
}

proc main() {
  var args = ["foo", "search", "--no-update-registry"];
  if pattern != "" then args.push_back(pattern);

  setEnv("MASON_CACHED_REGISTRY", "./registry");

  masonSearch(args);
}
