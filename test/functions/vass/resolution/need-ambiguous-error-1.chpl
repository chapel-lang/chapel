// a copy of one in ChapelBase.chpl
proc compilerError(param x:string ...?n) {
  __primitive("error", (...x));
}
compilerError("a", "a", "a", "a", "a");
