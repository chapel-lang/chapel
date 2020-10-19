/* Test a package with Manifest.toml without [dependencies] table */
use MasonBuild;
use MasonNew;
use MasonUtils;

use FileSystem;
use SysCTypes;

extern proc setenv(name : c_string, envval : c_string, overwrite : c_int) : c_int;

proc main() {
  const package = '_noDeps';

  const oldPWD = getEnv('PWD');

  here.chdir(package);
  const newPWD = '/'.join(oldPWD, package);
  setenv("PWD", newPWD.c_str(), 1);

  const args = ["mason", "build"];
  masonBuild(args);
}
