config type myType = int;
config const initval: myType = 71:myType;

var sx: sync myType;
var x: myType;
var y = 2*initval;

sync {
  begin with (ref x) {
    x = sx.readFE();
    if sx.isFull then writeln("invalid state (full)");
    if x != initval then writeln("incorrect value: ", x);
  }

  sx.writeEF(initval);
}
if sx.isFull then writeln("invalid state (full)");

begin {
  sx.writeEF(y);
  if sx.readXX() != 2*initval then writeln("incorrect value: ", sx.readXX());
}

x = sx.readFE();
if sx.isFull then writeln("invalid state (full)");
if x != 2*initval then writeln("incorrect value: ", x);

writeln("DONE");
