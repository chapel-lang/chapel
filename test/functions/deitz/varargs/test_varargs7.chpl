class C {
  param rank : int;
  proc this(ii : int ...rank) : int {
    for i in 0..#rank do
      writeln(ii(i));
    return 4;
  }
}

var ownC = new owned C(3);
var c = ownC.borrow();

writeln(c(1,2,3));
