class C {
  proc bbox(x: int): range(strides=strideKind.negOne) {
    halt("bbox() not implemented for this class");
    // return 0..-1 by -1; // this line would be ignored
  }
}

class E : C {
  param rank: int;
  var ranges : rank*range(int, boundKind.both, strideKind.negOne);

  proc postinit() {
    for i in 1..rank do
      ranges(i-1) = 1..i by -1;
  }

  override proc bbox(x: int) {
    return ranges(x-1);
  }
}

var ownE = new owned E(4);
var e:borrowed C = ownE.borrow();

writeln(e.bbox(1));
writeln(e.bbox(2));
writeln(e.bbox(3));
writeln(e.bbox(4));
