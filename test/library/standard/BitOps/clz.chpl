use BitOps;

var x: uint(8) = 0x08;
writeln(clz(x));
writeln(clz(x:uint(16)));
writeln(clz(x:uint(32)));
writeln(clz(x:uint(64)));
