use FileSystem;
use MasonUtils;
use MasonBuild;
use CTypes;
use List;

var path: string;
extern proc setenv(name: c_ptrConst(c_char),
                   envval: c_ptrConst(c_char), overwrite: c_int): c_int;
extern proc getenv(name: c_ptrConst(c_char)): c_ptrConst(c_char);
proc removePath() {
  path = string.createCopyingBuffer(getenv("PATH"));
  setenv("PATH", "", 1);
}
proc restorePath() {
  setenv("PATH", path.c_str(), 1);
}


proc main(){

  mkdir("srcTest");
  defer rmTree("srcTest");

  try {
    gitC("srcTest", "git ini", quiet=true);
  } catch e {
    writeln(e); // should get an unknown git error
  }

  try {
    removePath();
    defer restorePath();
    gitC("srcTest", "git init", quiet=true);
  } catch e {
    writeln(e); // should get a missing git error
  }

  var srcList = new list([
    new srcSource("this-url-will-not-exist", "dummy", "0.1.0"),
    new srcSource("this-url-will-not-exist-either", "dummy2", "0.1.0")]);
  var gitList = new list([
    new gitSource("this-url-will-not-exist", "dummy", "HEAD", "123456"),
    new gitSource("this-url-will-not-exist-either", "dummy2", "HEAD", "123456")]);

  try {
    getSrcCode(srcList, skipUpdate=false, show=false);
  } catch e {
    writeln(e); // should get errors
  }
  try {
    getSrcCode(srcList, skipUpdate=true, show=false);
  } catch e {
    writeln(e); // should get errors
  }
  try {
    getGitCode(gitList, skipUpdate=false, show=false);
  } catch e {
    writeln(e); // should get errors
  }
  try {
    getGitCode(gitList, skipUpdate=true, show=false);
  } catch e {
    writeln(e); // should get errors
  }

}
