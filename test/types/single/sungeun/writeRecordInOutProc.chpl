config type myType = int;
config const initval: myType = 71:myType;

record myR {
  var x: myType;
  var sx: single myType;
}

var r: myR;

r.x = 2*initval;

foo(r);
writeln(r); // This is a read of r.sx

proc foo(inout r: myR) {
  r.sx = 2*initval;
  writeln(r); // This is a read of r.sx
  r.x = initval;
}

