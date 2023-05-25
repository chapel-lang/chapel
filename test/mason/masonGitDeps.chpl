use MasonUtils;
use MasonBuild;
use TOML;

proc main() {
  const cwd = here.cwd();
  
  // parse lockfile
  const toParse = open(cwd + "/gitTest.lock", ioMode.r);
  const lockFile = parseToml(toParse);

  // get list of git dependencies from lock file
  const (sourceList, gitList) = genSourceList(lockFile);

  // generate includes to git repos
  const compopts = getMasonDependencies(sourceList,
                                        gitList,
                                        "gitTest");
  // ensure the git path to the two dependencies exist
  writeln(compopts.find("/git/DummyRepo-master/src/DummyRepo.chpl") > 0);
  writeln(compopts.find("/git/SecondRepo-test-branch/src/SecondRepo.chpl") > 0);
}
