

proc baz(arg) {
  arg = 1;
}

proc bar(arg) {
  begin on Locales[numLocales-1] {
    baz(arg);
  }
}

proc foo(arg) {
  bar(arg);
}

foo("hi");
