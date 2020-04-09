class C {
  proc bbox(x: int) {
    halt("bbox() not implemented for this class");
    return 0..-1;
  }
}

class D : C {
  param rank: int;
  var ranges : rank*range(int, BoundedRangeType.bounded, false);

  proc postinit() {
    for i in 1..rank do
      ranges(i-1) = 1..i;
  }

  override proc bbox(x: int) {
    return ranges(x-1);
  }
}

var d:borrowed C = new borrowed D(4);

writeln(d.bbox(1));
writeln(d.bbox(2));
writeln(d.bbox(3));
writeln(d.bbox(4));
