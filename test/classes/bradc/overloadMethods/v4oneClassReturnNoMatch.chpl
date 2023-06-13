class C {
  proc bbox(x: int) {
    halt("bbox() not implemented for this class");
    return 0..-1 by -1;
  }
}

class D : C {
  param rank: int;
  var ranges : rank*range(int, boundKind.both);

  proc postinit() {
    for i in 1..rank do
      ranges(i) = 1..i;
  }

  override proc bbox(x: int) {
    return ranges(x);
  }
}

var d:borrowed C = (new owned D(4)).borrow();
writeln(d.bbox(1));
writeln(d.bbox(2));
writeln(d.bbox(3));
writeln(d.bbox(4));
