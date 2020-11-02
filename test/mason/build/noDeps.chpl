/* Test a package with Manifest.toml without [dependencies] table */
use FileSystem;

use MasonBuild;

proc main() {
  const package = '_noDeps';
  here.chdir(package);
  const args = ["mason", "build"];
  masonBuild(args);
}
