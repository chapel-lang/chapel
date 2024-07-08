use MasonPublish;
use MasonEnv;
use FileSystem;


proc main() {
  const args = ["publish", "--refresh-licenses"];
  const spdxDir = MASON_HOME + '/spdx';
  const spdxTextDir = spdxDir + "/text";
  if exists(spdxTextDir) then rmTree(spdxTextDir);
  masonPublish(args);
}
