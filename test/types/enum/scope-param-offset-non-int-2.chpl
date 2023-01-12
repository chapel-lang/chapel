
param x = false;
proc enumScopeTest() {
  enum Numbers {one=x, two, three};
  writeln(Numbers.one);
}

enumScopeTest();
