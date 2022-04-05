use IO;

var u8:uint(64) = 0x1020304050607080;
var u4:uint(32) = 0x01020304;
var u2:uint(16) = 0x1122;
var u1:uint(8) = 0x7F;

var i8:int(64) = 0x1020304050607080;
var i4:int(32) = 0x01020304;
var i2:int(16) = 0x1122;
var i1:int(8) = 0x7F;

var r32:real(32) = 3.14159;
var r64:real(64) = 3.14159;

var im32:imag(32) = 3.14159i;
var im64:imag(64) = 3.14159i;

var x64:complex(64) = 3.14159 + 2.718i;
var x128:complex(128) = 3.14159 + 2.718i;

var endians = [ioendian.native, ioendian.little, ioendian.big];

proc output(arg:numeric) {
    for endian in endians {
        stdout.writeBinary(arg, endian);
    }
}

stdout.writeBinary(i1);
output(i1);
output(i2);
output(i4);
output(i8);

stdout.writeBinary(u1);
output(u1);
output(u2);
output(u4);
output(u8);

output(r32);
output(r64);

output(im32);
output(im64);

output(x64);
output(x128);
