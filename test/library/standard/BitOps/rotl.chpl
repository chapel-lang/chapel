use BitOps;

var x: uint(64) = 0xFFFFFFFF00000000;
writef("%@018xu\n", rotl(x, 17));

var x2: uint(32) = 0xFFFF0000;
writef("%@010xu\n", rotl(x2, 9));

var x3: uint(16) = 0xFF00;
writef("%@06xu\n", rotl(x3, 5));

var x4: uint(8) = 0xF0;
writef("%@04xu\n", rotl(x4, 3));

var x5: int(64) = 0x0FFFFFFF00000000;
writef("%@018xu\n", rotl(x5, 17));

var x6: int(32) = 0x0FFF0000;
writef("%@010xu\n", rotl(x6, 9));

var x7: int(16) = 0x0F00;
writef("%@06xu\n", rotl(x7, 5));

var x8: int(8) = 0x70;
writef("%@04xu\n", rotl(x8, 3));
