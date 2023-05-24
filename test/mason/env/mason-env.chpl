
use MasonEnv;

use FileSystem, CTypes;

proc setEnv(name : string, val : string) {
  extern proc setenv(name : c_string, val : c_string, overwrite : c_int) : c_int;

  const ret = setenv(c_ptrToConst_helper(name):c_string, c_ptrToConst_helper(val):c_string, 1);
  assert(ret == 0);
}

proc unsetEnv(name : string) {
  extern proc unsetenv(name : c_string) : c_int;
  const ret = unsetenv(c_ptrToConst_helper(name):c_string);
  assert(ret == 0);
}

proc main() {
  // Clear environment for testing
  unsetEnv("MASON_HOME");
  unsetEnv("MASON_REGISTRY");
  unsetEnv("MASON_OFFLINE");

  setEnv("MASON_HOME", here.cwd());

  const args = ["env"];
  const debugArgs = ["env", "--debug"];
  const helpArgs = ["env", "--help"];

  masonEnv(args);
  writeln("----------");

  setEnv("MASON_REGISTRY", "foobar|foobar");
  masonEnv(args);
  writeln("----------");
  unsetEnv("MASON_REGISTRY");

  setEnv("MASON_OFFLINE", "true");
	masonEnv(args);
  writeln("---------");
  unsetEnv("MASON_OFFLINE");

  masonEnv(debugArgs);
  writeln("----------");

  masonEnv(helpArgs);
}
