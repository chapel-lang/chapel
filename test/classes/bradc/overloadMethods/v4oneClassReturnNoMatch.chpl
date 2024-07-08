class C {
  proc bbox(x: int): range(strides=strideKind.negOne) {
    halt("bbox() not implemented for this class");
    // return 0..-1 by -1; // this line would be ignored
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

var dd = new owned D(4);
var d:borrowed C = dd.borrow();
writeln(d.bbox(1));
writeln(d.bbox(2));
writeln(d.bbox(3));
writeln(d.bbox(4));
