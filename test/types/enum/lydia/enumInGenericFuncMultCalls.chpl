enumScopeTest(2);
enumScopeTest(false);

proc enumScopeTest(x) {
  enum Numbers {one, two, three};
  writeln(Numbers.one);
}
