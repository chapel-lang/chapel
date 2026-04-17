use BitOps;

var x: uint(8) = 0x08;
writeln(clz(x));
writeln(clz(x:uint(16)));
writeln(clz(x:uint(32)));
writeln(clz(x:uint(64)));
writeln(clz(x:int(8)));
writeln(clz(x:int(16)));
writeln(clz(x:int(32)));
writeln(clz(x:int(64)));
