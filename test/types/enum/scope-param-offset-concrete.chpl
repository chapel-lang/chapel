enumScopeTest();
param x = 2;
proc enumScopeTest() {
  enum Numbers {one=x, two, three};
  writeln(Numbers.one);
}
