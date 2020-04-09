// generate a call stack as the context for an error message

test1("test1");

proc test1(x) {
  test2(x);
}

proc test2(y) {
  test3(y);
}

proc test3(z) {
  compilerError("error from test3, depth=0", 0);
}
