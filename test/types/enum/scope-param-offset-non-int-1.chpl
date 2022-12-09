
enumScopeTest("hi");
enumScopeTest(false);

proc enumScopeTest(param x) {
  enum Numbers {one=x, two, three};
  writeln(Numbers.one);
}
