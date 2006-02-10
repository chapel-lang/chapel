class C {
  function this(ii : integer ...3) : integer {
    for i in 1..3 do
      writeln(ii(i));
    return 4;
  }
}

var c = C();

writeln(c(1,2,3));
