config type myType = int;
config const initval: myType = 71:myType;

record myR {
  var x: myType;
  var sx: sync myType;

  proc init=(other: myR) {
    this.x = other.x;
    this.complete();
    this.sx.writeEF(other.sx.readFE());
  }
}

proc =(ref lhs: myR, rhs: myR) {
  lhs.x = rhs.x;
  lhs.sx.writeEF(rhs.sx.readFE());
}

var r: myR;

r.x = 2*initval;
r.sx.writeEF(initval);

foo(r);

proc foo(inout r: myR) {
  writeln(r.sx.readFE()); // This is a read of r.sx
  r.x = initval;
  r.sx.writeEF(2*initval);
  writeln(r.sx.readFF()); // Keep full so it can be copied into the actual
}

