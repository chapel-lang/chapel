use FileSystem;
use MasonUtils;
use CTypes;

proc removePath() {
  extern proc setenv(name: c_ptrConst(c_char), envval: c_ptrConst(c_char), overwrite: c_int): c_int;
  setenv("PATH", "", 1);
}

proc main(){

  mkdir("srcTest");
  defer rmTree("srcTest");

  try {
    gitC("srcTest", "git ini");
  } catch e {
    writeln(e); // should get an unknown git error
  }

  try {
    removePath();
    gitC("srcTest", "git init");
  } catch e {
    writeln(e); // should get a missing git error
  }

}
