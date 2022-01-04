config type myType = int;
config const initval: myType = 71:myType;

var sx: single myType;
var x: myType;

begin {
  var x: myType = sx.readFF();
  if !sx.isFull then writeln("invalid state (empty)");
  if x != initval then writeln("incorrect value: ", x, ", should be ", initval);
}

sx.writeEF(initval);
if !sx.isFull then writeln("invalid state (empty)");
if sx.readXX() != initval then writeln("incorrect value: ", sx.readXX(), ", should be ", initval);
  
x = 2*sx.readFF();
if !sx.isFull then writeln("invalid state (empty)");
if x != 2*initval then writeln("incorrect value: ", x, ", should be ", 2*initval);

writeln("DONE");
