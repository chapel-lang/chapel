config type myType = int;
config const initval: myType = 71:myType;

var sx: single myType;
var x: myType;

proc readme(sx: single myType) {
  var x = sx.readFF();
}

proc writeme(sx: single myType) {
  sx.writeEF(initval);
  if !sx.isFull then writeln("invalid state (empty)");
  if sx.readXX() != initval then writeln("incorrect value: ", sx.readXX());
}

writeme(sx);
if !sx.isFull then writeln("invalid state (empty)");
if sx.readXX() != initval then writeln("incorrect value: ", sx.readXX());

readme(sx);
if !sx.isFull then writeln("invalid state (empty)");

writeln("DONE");
