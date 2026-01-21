use FileSystem, Path, IO, Time, CTypes;
use MasonBuild;

proc setEnv(name: string, val: string) {
  extern proc setenv(name: c_ptrConst(c_char),
                     val: c_ptrConst(c_char), overwrite: c_int): c_int;

  const ret = setenv(name.c_str(), val.c_str(), 1);
  assert(ret == 0);
}

proc main() {
  const package = '_subModule';
  here.chdir(package);

  // initial build
  masonBuild(["build"]);

  sleep(2); // ensure mod time difference

  // do it again, nothing should happen
  masonBuild(["build"]);

  sleep(2); // ensure mod time difference

  // rebuild with a compopt to force a rebuild
  masonBuild(["build", "--", "--fast"]);

  sleep(2); // ensure mod time difference

  // do it again, nothing should happen
  masonBuild(["build", "--", "--fast"]);

  sleep(2); // ensure mod time difference

  // initial build of examples
  masonBuild(["build", "--example", "hello.chpl"]);

  sleep(2); // ensure mod time difference

  // do it again, nothing should happen
  masonBuild(["build", "--example", "hello.chpl"]);

  sleep(2); // ensure mod time difference

  // rebuild with -O1
  setEnv("CHPL_CC_FLAGS", "-O1");
  masonBuild(["build", "--example", "hello.chpl"]);

  sleep(2); // ensure mod time difference

  // do it again, nothing should happen
  masonBuild(["build", "--example", "hello.chpl"]);
}
