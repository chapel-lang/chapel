class C {
  proc bbox(x: int) {
    halt("bbox() not implemented for this class");
    return 0..-1 by 1;
  }
}

class E : C {
  param rank: int;
  var ranges : rank*range(int, BoundedRangeType.bounded, true);

  proc postinit() {
    for i in 1..rank do
      ranges(i) = 1..i by -1;
  }

  proc bbox(x: int) {
    return ranges(x);
  }
}

var e:C = new E(4);

writeln(e.bbox(1));
writeln(e.bbox(2));
writeln(e.bbox(3));
writeln(e.bbox(4));

delete e;
