use MasonUtils;
use MasonBuild;
use TOML;

proc main() {
  const cwd = here.cwd();
  
  // parse lockfile
  const toParse = open(cwd + "/gitTest.lock", iomode.r);
  const lockFile = parseToml(toParse);

  // get list of git dependencies from lock file
  const (sourceList, gitList) = genSourceList(lockFile);

  // generate includes to git repos
  const compopts = getMasonDependencies(sourceList,
                                        gitList,
                                        "gitTest");
  writeln(compopts);
}