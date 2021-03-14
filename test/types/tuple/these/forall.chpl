config var x = 1.0;
config var y = -1.0;
config var z = 83.0;

var A = [x, y, z];
var B = [y, z, x];
var C = [z, x, y];

forall e in (x,y,z) do writeln(e);

forall e in (A,B,C) do writeln(e);

var t = (A,B,C);
forall e in t do writeln(e);

config param s = 33;
var t1: s*x.type;
var sum: atomic x.type;

forall e in t1 do sum.fetchAdd(1);

if sum.read() != s:x.type then halt("Sum should be ", s, "!");
