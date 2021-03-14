enumScopeTest(2);

proc enumScopeTest(x) {
  enum Numbers {one, two, three};

  var first = Numbers.one;
  var other = Numbers.three;
  writeln(first);
  writeln(other);
  other = first;
  writeln(other);
}
