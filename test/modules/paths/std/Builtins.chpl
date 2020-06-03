module Builtins {
  extern proc printf(x...);
  printf("In my personal copy of Builtins\n");
  
  proc assert(test: bool) {
    if !test then
      printf("In my assert, got false\n");
  }

  proc assert(test: bool, args ...?numArgs) {
    if !test then
      printf("In my assert, got false and some args\n");
  }

  proc compilerError(param msg: string ...?n, param errorDepth: int) {
    __primitive("error");
  }

  proc compilerError(param msg: string ...?n) {
    __primitive("error");
  }

  proc compilerWarning(param msg: string ...?n, param errorDepth: int) {
    __primitive("warning");
  }

  proc compilerWarning(param msg: string ...?n) {
    __primitive("warning");
  }

  proc compilerAssert(param test: bool) { 
    if !test then compilerError("assert failed");
  }

  proc compilerAssert(param test: bool, param errorDepth: int) {
    if !test then compilerError("assert failed", errorDepth + 1);
  }

  proc compilerAssert(param test: bool, param msg: string ...?n) {
    if !test then compilerError("assert failed - ", (...msg));
  }

  proc compilerAssert(param test: bool, param msg: string ...?n,
                      param errorDepth: int) {
    if !test then compilerError("assert failed - ", (...msg), errorDepth + 1);
  }
}
