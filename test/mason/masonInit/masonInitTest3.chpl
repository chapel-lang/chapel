use MasonInit;
use CTypes;

proc removePath() {
  extern proc setenv(name: c_ptrConst(c_char), envval: c_ptrConst(c_char), overwrite: c_int): c_int;
  setenv("PATH", "", 1);
}

proc main(){
  removePath(); // clobber path so error occurs
  const args = ['init'];
  masonInit(args);
}
