proc foo(x: string) {
  compilerWarning("In execution-time string foo()");
}

proc foo(param x: string) {
  compilerWarning("In compile-time string foo()");
}

proc bar(x: int) {
  compilerWarning("In execution-time int bar()");
}

proc bar(param x: int) {
  compilerWarning("In compile-time int bar()");
}

foo("Chapel"[1]);
bar(b"Chapel"[1]);
