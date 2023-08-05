config type myType = int;
config const initval: myType = 71:myType;

record myR {
  var x: myType;
  var sx: single myType;
  proc init() {}
  proc init=(other: myR) {
    this.x = other.x;
    if other.sx.isFull then
      this.sx = other.sx.readFF();
  }
  proc writeThis(wri) throws {
    wri.write("(x = ", x, ", sx = ", sx.readFF(), ")");
  }
}

operator myR.=(ref lhs: myR, rhs: myR) {
  lhs.x = rhs.x;
  if rhs.sx.isFull then
    lhs.sx.writeEF(rhs.sx.readFF());
}

var r: myR;

r.x = 2*initval;

foo(r);
writeln(r); // This is a read of r.sx

proc foo(out r: myR) {
  r.sx.writeEF(2*initval);
  writeln(r); // This is a read of r.sx
  r.x = initval;
}

