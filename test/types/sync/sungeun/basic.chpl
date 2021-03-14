config type myType = int;
config const initval: myType = 71:myType;

var sx: sync myType;
sx = initval;
if !sx.isFull then writeln("invalid state (empty)");
if sx.readXX() != initval then writeln("incorrect value: ", sx.readXX());

var x: myType;
x = sx.readFE();
if sx.isFull then writeln("invalid state (full)");
if x != initval then writeln("incorrect value: ", x);

sx.writeEF(2*x);
if !sx.isFull then writeln("invalid state (empty)");
if sx.readXX() != 2*initval then writeln("incorrect value: ", sx.readXX());

x = sx.readFE();
if sx.isFull then writeln("invalid state (full)");
if x != 2*initval then writeln("incorrect value: ", x);

writeln("DONE");
