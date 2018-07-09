enumScopeTest(2);

proc enumScopeTest(x) {
  enum Numbers {one=1, two, three};

  for i in Numbers {
    var x = i: int;
    writeln(i, ": ", x);
  }
}
