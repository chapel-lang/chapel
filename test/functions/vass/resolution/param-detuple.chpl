
proc p1(args...) {
  compilerWarning("non-param proc");
}
proc p1(param args...) {
  compilerWarning("param proc");
}
proc p2(param args...) {
  p1((...args));
}

p2("a");
p2("a","b");
compilerAssert(false, "done", 2); // exercise stack-not-deep-enough warning
