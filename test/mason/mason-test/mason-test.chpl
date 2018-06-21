use MasonTest;


use FileSystem;

proc setEnv(name : string, val : string) {
  extern proc setenv(name : c_string, val : c_string, overwrite : c_int) : c_int;

  const ret = setenv(name.c_str(), val.c_str(), 1);
  assert(ret == 0);
}


proc main() {
  const args = ["mason", "test"];
  setEnv("PWD", here.cwd()); 
  masonTest(args);
}
