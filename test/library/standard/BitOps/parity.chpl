use BitOps;

var x: uint = 0xFF773311FF773311;
writeln(parity(x));
x = 0xFF773311FF773313;
writeln(parity(x));

var x2: uint(32) = 0xF731F731;
writeln(parity(x2));
x2 = 0xF731F733;
writeln(parity(x2));

var x3: uint(16) = 0xF731;
writeln(parity(x3));
x3 = 0xF733;
writeln(parity(x3));

var x4: uint(8) = 0x0F;
writeln(parity(x4));
x4 = 0x1F;
writeln(parity(x4));
