class C {
  def bbox(x: int) {
    halt("bbox() not implemented for this class");
    return 0..-1 by 1;
  }
}

class D : C {
  param rank: int;
  var ranges : rank*range(int, bounded, false);

  def initialize() {
    for i in 1..rank do
      ranges(i) = 1..i;
  }

  def bbox(x: int) {
    return ranges(x);
  }
}

class E : C {
  param rank: int;
  var ranges : rank*range(int, bounded, true);

  def initialize() {
    for i in 1..rank do
      ranges(i) = 1..i by -1;
  }
  
  def bbox(x: int) {
    return ranges(x);
  }
}

var d:C = new D(4);
writeln(d.bbox(1));
writeln(d.bbox(2));
writeln(d.bbox(3));
writeln(d.bbox(4));

var e:C = new E(4);
writeln(e.bbox(1));
writeln(e.bbox(2));
writeln(e.bbox(3));
writeln(e.bbox(4));
