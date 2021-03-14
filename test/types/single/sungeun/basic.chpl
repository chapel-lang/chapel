config type myType = int;
config const initval: myType = 71:myType;

var sx: single myType;
sx = initval;
if !sx.isFull then writeln("invalid state (empty)");
if sx.readXX() != initval then writeln("incorrect value: ", sx.readXX());

var x: myType;
x = sx.readFF();
if !sx.isFull then writeln("invalid state (empty)");
if x != initval then writeln("incorrect value: ", x);
  
x = 2*sx.readFF();
if !sx.isFull then writeln("invalid state (empty)");
if x != 2*initval then writeln("incorrect value: ", x);

writeln("DONE");
