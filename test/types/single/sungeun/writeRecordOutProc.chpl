config type myType = int;
config const initval: myType = 71:myType;

record myR {
  var x: myType;
  var sx: single myType;
  proc writeThis(wri) throws {
    wri.write("(x = ", x, ", sx = ", sx.readFF(), ")");
  }
}

var r: myR;

r.x = 2*initval;

foo(r);
writeln(r); // This is a read of r.sx

proc foo(out r: myR) {
  r.sx = 2*initval;
  writeln(r); // This is a read of r.sx
  r.x = initval;
}

