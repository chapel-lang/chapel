enumScopeTest(2);

proc enumScopeTest(x) {
  enum Numbers {one, two, three};

  for i in Numbers {
    var x : int;
    x = i;
    writeln(i, ": ", x);
  }
}
