class C {
  proc bbox(d: int) {
    halt("bbox() is not implemented for this class");
    return 1..0;
  }
}


class D : C {
  param rank: int;
  type dim_type;
  param stridable: bool;

  var ranges: rank*range(dim_type, BoundedRangeType.bounded, stridable);

  override proc bbox(d: int) {
    const r: range(dim_type, BoundedRangeType.bounded, false) = ranges(d-1);
    return r;
  }
}

class E : C {
  var parentDom: unmanaged C;

  var rowRange = parentDom.bbox(1);
}




var myD = new unmanaged D(2, int, false);

myD.ranges(0) = 1..10;
myD.ranges(1) = 1..20;

var r1 = myD.bbox(1);
var r2 = myD.bbox(2);

writeln("r1 = ", r1);
writeln("r2 = ", r2);





var myE = new unmanaged E(myD);

writeln(myE.rowRange);

delete myD;
delete myE;
