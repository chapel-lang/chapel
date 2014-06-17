config type myType = int;
config const initval: myType = 71:myType;

record myR {
  var x: myType;
  var sx: sync myType;
}

var r: myR;

r.x = 2*initval;
r.sx = initval;

foo(r);

proc foo(inout r: myR) {
  writeln(r); // This is a read of r.sx
  r.x = initval;
  r.sx = 2*initval;
  writeln(r); // This is a read of r.sx
}

