enumScopeTest(2);

proc enumScopeTest(param x) {
  enum Numbers {one, two, three};
  writeln(Numbers.one);
}
