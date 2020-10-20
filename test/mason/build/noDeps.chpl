/* Test a package with Manifest.toml without [dependencies] table */
use MasonBuild;
use MasonNew;
use MasonUtils;

use FileSystem;
use SysCTypes;

extern proc setenv(name : c_string, envval : c_string, overwrite : c_int) : c_int;

// chdir and reset PWD env (some mason internals rely on this)
proc changeDir(relPath: string) {
  const oldPWD = getEnv('PWD');
  here.chdir(relPath);
  const newPWD = '/'.join(oldPWD, relPath);
  setenv("PWD", newPWD.c_str(), 1);
}

proc main() {
  const package = '_noDeps';
  changeDir(package);
  const args = ["mason", "build"];
  masonBuild(args);
}
