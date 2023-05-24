
use List;
use MasonEnv;
use MasonSearch;
use MasonUtils;

use FileSystem;
use Subprocess;
use CTypes;

proc setEnv(name : string, val : string) {
  extern proc setenv(name : c_string, val : c_string, overwrite : c_int) : c_int;

  const ret = setenv(name.c_str(), val.c_str(), 1);
  assert(ret == 0);
}

proc unsetEnv(name : string) {
  extern proc unsetenv(name : c_string) : c_int;
  const ret = unsetenv(name.c_str());
  assert(ret == 0);
}

proc makeToml(name: string, ver: string) {
  if isDir(name) == false {
    mkdir(name);
  }

  var fi = open(name + "/" + ver + ".toml", ioMode.cw);
  var w  = fi.writer();
  const info = "\n" +
"[brick]\n" +
"name = '" + name + "'\n" +
"version = '" + ver + "'\n" +
"chplVersion = '1.16.0'\n" +
"author = 'foo'\n" +
"source = 'nothing'\n" +
"\n" +
"[dependencies]\n";

  w.write(info);
  w.close();
  fi.close();
}

proc buildFakeRegistry(regDir : string) {
  const oldCWD = here.cwd();

  assert(isDir(regDir) == false);
  mkdir(regDir);
  here.chdir(regDir);

  runCommand("git init", true);

  mkdir(regDir + "/Bricks/");
  here.chdir(regDir + "/Bricks/");
  makeToml("simple", "0.1.0");
  makeToml("multiple", "0.1.0");
  makeToml("multiple", "0.2.0");
  here.chdir(regDir);

  runCommand("git add .", true);
  runCommand("git commit -m \"nothing\"", true);

  here.chdir(oldCWD);
}

proc main() {
  // Clear environment for testing
  unsetEnv("MASON_HOME");
  unsetEnv("MASON_REGISTRY");
  unsetEnv('MASON_OFFLINE');

  const tempHome = here.cwd() + "/tempHome";
  const uncached = "/uncached";
  const altRegistry = tempHome + uncached + "/mason-registry";
  const altRegistryName = "mason-registry";

  mkdir(tempHome + uncached, parents=true);

  setEnv("MASON_HOME", tempHome);
  setEnv("MASON_REGISTRY", altRegistryName + "|" + altRegistry);

  buildFakeRegistry(altRegistry);

  masonEnv(["env"]);

  var args1: list(string);
  for x in ["search"] do args1.pushBack(x);
  masonSearch(args1);

  assert(isDir(altRegistry));
  assert(isDir(tempHome));
  rmTree(tempHome);
}
