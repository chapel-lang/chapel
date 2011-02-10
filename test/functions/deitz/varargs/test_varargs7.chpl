class C {
  param rank : int;
  proc this(ii : int ...rank) : int {
    for i in 1..rank do
      writeln(ii(i));
    return 4;
  }
}

var c = new C(3);

writeln(c(1,2,3));

delete c;
