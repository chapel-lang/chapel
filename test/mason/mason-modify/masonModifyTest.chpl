use MasonModify;
use MasonUtils;
use MasonSearch;

use TOML;
use FileSystem;

use SysCTypes;
extern proc setenv(name : c_string, envval : c_string, overwrite : c_int) : c_int;

/* Copy the toml file into a package (empty directory) as 'Mason.toml',
   modify it with the provided arguments, and print it out.
 */
proc main(args: [] string) {

  // Expect basename as first arg, e.g. addDep
  const basename = args[1];

  // Wipe a pre-existing project
  if exists(basename) then rmTree(basename);

  // Create new mason project directory
  mkdir(basename);

  // Replace manifest file
  const tomlFile = basename + '.toml';
  const manifestFile = basename + '/Mason.toml';
  FileSystem.copyFile(tomlFile, manifestFile);

  // here.chdir is not sufficient
  const oldPWD = getEnv('PWD');
  const newPWD = '/'.join(oldPWD, basename);
  setenv("PWD", newPWD.c_str(), 1);

  // Add mason dependency
  var modArgs: [0..#args.size-2] string = args[2..];
  masonModify(modArgs);;

  // Print manifest for diff against .good file
  showToml(manifestFile);

  // Cleanup project
  rmTree(basename);
}
