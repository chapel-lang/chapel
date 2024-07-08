use MasonModify;
use MasonUpdate;
use MasonUtils;

use TOML;
use FileSystem;

proc main() {
  const basename = "addBadDep";
  mkdir('tmp/' + basename, parents=true);

  const tomlFile = basename + '.toml';
  const manifestFile = 'tmp/' + basename + '/Mason.toml';
  FileSystem.copy(tomlFile, manifestFile, permissions=false);

  here.chdir('tmp/' + basename);

  var addCmd: [0..#2] string;
  addCmd[0] = "add";
  addCmd[1] = "badDep@5.5.5";
  
  masonModify(addCmd);
  showToml('Mason.toml');

  here.chdir('../../');
  
  rmTree('tmp/');
}
