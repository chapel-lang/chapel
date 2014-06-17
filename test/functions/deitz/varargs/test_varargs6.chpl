class C {
  proc this(ii : int ...3) : int {
    for i in 1..3 do
      writeln(ii(i));
    return 4;
  }
}

var c = new C();

writeln(c(1,2,3));

delete c;
