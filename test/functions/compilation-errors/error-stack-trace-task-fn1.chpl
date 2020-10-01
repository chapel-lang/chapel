

proc baz(arg) {
  arg = 1;
}

proc bar(arg) {
  on Locales[numLocales-1] {
    baz(arg);
  }
}

proc foo(arg) {
  bar(arg);
}

foo("hi");
