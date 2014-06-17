var x: uint(64) = 1;
//var y: int = 1;

proc foo(x: uint(64), y: uint(64)) {
  writeln("Got x = ", x, " and y = ", y);
}

foo(x, 1);
var z1 = x + 1;

writeln("z1 is: ", z1);
