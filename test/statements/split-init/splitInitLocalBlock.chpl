var atomicVar1 : atomic int;
var atomicVar2 : atomic int;
var x;
var y;
local {
   x = atomicVar1.read();
   y = atomicVar2.read();
}
writeln(x, y);
