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
  // note that we are passing args from pos 3 because the execopts file
  // specifies an extra test name argument i.e:
  // testName mason add dep@1.2.3 # testName
  // so in this case the value of args would be:
  // ["./masonModifyTest", "addDep", "mason", "add", "test@1.2.3"]
  // but our subcommand should only see:
  // ["add","test@1.2.3"]
  var modArgs: [0..#args.size-3] string = args[3..];
  masonModify(modArgs);

  // Print manifest for diff against .good file
  showToml('Mason.toml');
}
