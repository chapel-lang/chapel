enumScopeTest(2);

proc enumScopeTest(x) {
  enum Numbers {one, two, three};

  var first = 1:Numbers;
  writeln(first);
}
