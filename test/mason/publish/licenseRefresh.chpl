use MasonPublish;
use MasonEnv;
use MasonUtils;
use FileSystem;
use Time;


proc main() {
  const args = ["publish", "--refresh-licenses"];
  const spdxDir = MASON_HOME + '/spdx';
  const spdxTextDir = spdxDir + "/text";

  // creates a new dir and clones when it doesn't exist, without overwrite flag
  if exists(spdxDir) then rmTree(spdxDir);
  writeln("spdx dir does not exist, creating it...");
  refreshLicenseList();

  assert(exists(spdxDir));
  assert(exists(spdxTextDir));
  writeln("spdx dir was created, removing the text/ subdir...");
  rmTree(spdxTextDir);

  assert(!exists(spdxTextDir));


  try! {
    refreshLicenseList(); // expect this to error because we deleted the text dir
  } catch e: MasonError {
    assert(!exists(spdxTextDir));
    writeln("spdx/text dir was not recreated because we did not force an overwrite");
  }

  sleep(5); // sleep a little to let git settle from the previous clone above
  refreshLicenseList(true); // set the overwrite flag to force getting the repo again
  assert(exists(spdxTextDir));
  writeln("spdx/text dir was recreated with use of overwrite flag");
}
