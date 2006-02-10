class C {
  param rank : integer;
  function this(ii : integer ...rank) : integer {
    for i in 1..rank do
      writeln(ii(i));
    return 4;
  }
}

var c = C(3);

writeln(c(1,2,3));
