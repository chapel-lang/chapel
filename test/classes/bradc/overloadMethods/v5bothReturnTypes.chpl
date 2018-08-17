class C {
  proc bbox(x: int) {
    halt("bbox() not implemented for this class");
    return 0..-1 by 1;
  }
}

class D : C {
  param rank: int;
  var ranges : rank*range(int, BoundedRangeType.bounded, false);

  proc postinit() {
    for i in 1..rank do
      ranges(i) = 1..i;
  }

  override proc bbox(x: int) {
    return ranges(x);
  }
}

class E : C {
  param rank: int;
  var ranges : rank*range(int, BoundedRangeType.bounded, true);

  proc postinit() {
    for i in 1..rank do
      ranges(i) = 1..i by -1;
  }
  
  override proc bbox(x: int) {
    return ranges(x);
  }
}

var d:borrowed C = new borrowed D(4);
writeln(d.bbox(1));
writeln(d.bbox(2));
writeln(d.bbox(3));
writeln(d.bbox(4));

var e:borrowed C = new borrowed E(4);
writeln(e.bbox(1));
writeln(e.bbox(2));
writeln(e.bbox(3));
writeln(e.bbox(4));
