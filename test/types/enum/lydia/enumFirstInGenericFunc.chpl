enumScopeTest(2);

proc enumScopeTest(x) {
  enum Numbers {one, two, three};
  writeln(chpl_enum_first(Numbers));
}
