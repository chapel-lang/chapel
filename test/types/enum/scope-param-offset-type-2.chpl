enumScopeTest(2);
enumScopeTest(3);

proc enumScopeTest(param x:int) {
  enum Numbers {one=x, two, three};
  writeln(Numbers.one);
}
