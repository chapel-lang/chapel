config type myType = int;
config const initval: myType = 71:myType;

record myR {
  var x: myType;
  var sx: sync myType;
}

var r: myR;

r.x = 2*initval;
r.sx = initval;
writeln(r.sx.readFE()); // This is a read of r.sx

// r.sx needs to be empty to allow foo() to assign into 'r'
foo(r);
writeln(r.sx.readFF());

proc foo(out r: myR) {
  r.x = initval;
  r.sx = 2*initval;
}

