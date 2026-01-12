use MasonInit;
use CTypes;
use MasonUtils;

proc removePath() {
  extern proc setenv(name: c_ptrConst(c_char), envval: c_ptrConst(c_char), overwrite: c_int): c_int;
  setenv("PATH", "", 1);
}

proc main() throws {
  removePath(); // clobber path so error occurs
  const args = ['init'];
  try {
    masonInit(args);
  } catch e: MasonError {
    writeln(e.message());
  }
}
