class C {
  proc bbox(x: int): range {
    halt("bbox() not implemented for this class");
  }
}

class D : C {
  param rank   : int;
  var   ranges : rank * range(int, BoundedRangeType.bounded, false);

  proc init(param rankVal: int) {
    rank = rankVal;

    this.complete();

    for i in 0..<rank do
      ranges(i) = 1..i+1;
  }

  override proc bbox(x: int) {
    return ranges(x);
  }
}

var d: C = new D(4);

writeln(d.bbox(0));
writeln(d.bbox(1));
writeln(d.bbox(2));
writeln(d.bbox(3));
