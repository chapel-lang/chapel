config type myType = int;
config const initval: myType = 71:myType;

var sx: sync myType;
var x: myType;

proc readme(sx: sync myType) {
  var x = sx.readFE();
}

proc writeme(sx: sync myType) {
  sx.writeEF(initval);
  if !sx.isFull then writeln("invalid state (empty)");
  if sx.readXX() != initval then writeln("incorrect value: ", sx.readXX());
}

sx = initval;
if !sx.isFull then writeln("invalid state (empty)");
if sx.readXX() != initval then writeln("incorrect value: ", sx.readXX());
readme(sx);
if sx.isFull then writeln("invalid state (full)");

sx.reset();
if sx.isFull then writeln("invalid state (full)");
if sx.readXX() != 0 then writeln("incorrect value: ", sx.readXX());
writeme(sx);
if !sx.isFull then writeln("invalid state (empty)");
if sx.readXX() != initval then writeln("incorrect value: ", sx.readXX());

writeln("DONE");
