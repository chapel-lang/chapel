enumScopeTest(2);
enumScopeTest(3);

proc enumScopeTest(param x) {
  enum Numbers {one=x:int, two, three};
  writeln(Numbers.one);
}
