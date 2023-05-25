class C {
  param rank : int;
  proc this(ii : int ...rank) : int {
    for i in 0..#rank do
      writeln(ii(i));
    return 4;
  }
}

var c = (new owned C(3)).borrow();

writeln(c(1,2,3));
