

use MasonEnv;
use MasonUtils;

use FileSystem, CTypes;

proc setEnv(name : string, val : string) {
  extern proc setenv(name : c_ptrConst(c_char), val : c_ptrConst(c_char), overwrite : c_int) : c_int;

  const ret = setenv(name.c_str(), val.c_str(), 1);
  assert(ret == 0);
}

proc unsetEnv(name : string) {
  extern proc unsetenv(name : c_ptrConst(c_char)) : c_int;
  const ret = unsetenv(name.c_str());
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

  // Test: Duplicate registry names should trigger error
  setEnv("MASON_REGISTRY", "foo|/tmp/registry1,foo|/tmp/registry2");
  try {
    var regs = MASON_REGISTRY;
    writeln("Test failed: No error thrown for duplicate registry names");
  } catch e: MasonError {
    if e.message().find("must be unique") >= 0 {
      writeln("Test passed: Caught expected error for duplicate registry names");
    } else {
      writeln("Test failed: Unexpected MasonError: ", e.message());
    }
  } catch {
    writeln("Test failed: Unexpected error type");
  }
  unsetEnv("MASON_REGISTRY");

  setEnv("MASON_OFFLINE", "true");
	masonEnv(args);
  writeln("---------");
  unsetEnv("MASON_OFFLINE");

  masonEnv(debugArgs);
  writeln("----------");

  masonEnv(helpArgs);
}
