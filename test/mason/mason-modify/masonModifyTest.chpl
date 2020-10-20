use MasonModify;
use MasonUtils;
use MasonSearch;

use TOML;
use FileSystem;

/* Copy the toml file into a package (empty directory) as 'Mason.toml',
   modify it with the provided arguments, and print it out.
 */
proc main(args: [] string) {

  // Expect basename as first arg, e.g. addDep
  const basename = args[1];

  // Wipe a pre-existing project
  if exists(basename) then rmTree(basename);

  // Create new mason project directory
  mkdir('tmp/' + basename, parents=true);

  // Replace manifest file
  const tomlFile = basename + '.toml';
  const manifestFile = 'tmp/' + basename + '/Mason.toml';
  FileSystem.copyFile(tomlFile, manifestFile);

  here.chdir('tmp/' + basename);

  // Add mason dependency
  var modArgs: [0..#args.size-2] string = args[2..];
  masonModify(modArgs);;

  // Print manifest for diff against .good file
  showToml('Mason.toml');
}
