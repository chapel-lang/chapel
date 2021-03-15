/* Test a package with Manifest.toml without [dependencies] table */
use FileSystem;

use MasonBuild;
use MasonUtils;

proc main() {
  const package = '_noDeps';
  var cwd = here.cwd();
  var tf = "Mason.toml";
  var projectHome = getProjectHome(cwd + '/' + package, tf);
  removeHash(projectHome, tf);

  here.chdir(package);
  const args = ["mason", "build"];
  masonBuild(args);
}
