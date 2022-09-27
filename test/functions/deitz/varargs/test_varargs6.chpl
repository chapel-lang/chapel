class C {
  proc this(ii : int ...3) : int {
    for i in 0..2 do
      writeln(ii(i));
    return 4;
  }
}

var c = (new owned C()).borrow();

writeln(c(1,2,3));
