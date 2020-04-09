enumScopeTest(2);

proc enumScopeTest(x) {
  enum Numbers {one, two, three};
  writeln(Numbers.size);
}
