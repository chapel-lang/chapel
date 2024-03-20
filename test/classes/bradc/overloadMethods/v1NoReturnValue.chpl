class C {
  proc bbox(x: int) {
    halt("bbox() not implemented for this class");
  }
}

class D : C {
  param rank: int;
  var ranges : rank*range(int, boundKind.both);

  proc init(param rankVal: int) {
    rank = rankVal;
    init this;
    for i in 1..rank do
      ranges(i) = 1..i;
  }

  proc bbox(x: int) {
    return ranges(x);
  }
}

var dd = new D(4); var d:borrowed C = dd.borrow();
writeln(d.bbox(1));
writeln(d.bbox(2));
writeln(d.bbox(3));
writeln(d.bbox(4));
