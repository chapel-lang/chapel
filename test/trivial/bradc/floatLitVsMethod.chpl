proc int.e {
  writeln("In int .e method");
  return this+1;
}

var w = 1.0e+1;
var x = 1.e+1;
var y = 1 .e+1;
var z = 1. e+1;


writeln("w = ", w);
writeln("x = ", x);
writeln("y = ", y);
writeln("z = ", z);
