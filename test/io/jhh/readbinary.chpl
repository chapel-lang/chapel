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
 
proc readone(expected:numeric, endian:ioendian=ioendian.native) {
    var inp: expected.type;
    stdin.readBinary(inp, endian);
    assert(inp == expected, inp.type:string, inp, expected);
}

proc readall(expected:numeric) {
    for endian in endians {
        readone(expected, endian);
    }
}

readone(i1);
readall(i1);
readall(i2);
readall(i4);
readall(i8);

readone(u1);
readall(u1);
readall(u2);
readall(u4);
readall(u8);

readall(r32);
readall(r64);

readall(im32);
readall(im64);

readall(x64);
readall(x128);
