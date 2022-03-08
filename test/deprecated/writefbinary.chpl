config type t = string;

var u8:uint(64) = 0x1020304050607080;
var u4:uint(32) = 0x01020304;
var u2:uint(16) = 0x1122;
var u1:uint(8) = 0x7F;

var i8:int(64) = 0x1020304050607080;
var i4:int(32) = 0x01020304;
var i2:int(16) = 0x1122;
var i1:int(8) = 0x7F;


writef("%|1i %|1i":t, i1, i1);
writef("%<1i %>1i":t, i1, i1);
writef("%<2i %>2i":t, i2, i2);
writef("%<4i %>4i":t, i4, i4);
writef("%<8i %>8i":t, i8, i8);

writef("%|1u %|1u":t, u1, u1);
writef("%<1u %>1u":t, u1, u1);
writef("%<2u %>2u":t, u2, u2);
writef("%<4u %>4u":t, u4, u4);
writef("%<8u %>8u":t, u8, u8);


var test = "test";

writef("A":t);
writef("%>4s":t, test);
writef("%>*s":t, 4, test);
writef("B":t);
writef("%>1S %>2S %>4S %>8S %>vS":t, test, test, test, test, test);
writef("C":t);
writef("%|1S %|vS":t, test, test);
writef("D":t);
writef("%>0S":t, test);
writef("%>*S":t, 1, test);

var testBytes = "test":bytes;

writef("A":t);
writef("%>4s":t, testBytes);
writef("%>*s":t, 4, testBytes);
writef("B":t);
writef("%>1S %>2S %>4S %>8S %>vS":t, testBytes, testBytes, testBytes, testBytes,
    testBytes);
writef("C":t);
writef("%|1S %|vS":t, testBytes, testBytes);
writef("D":t);
writef("%>0S":t, testBytes);
writef("%>*S":t, 1, testBytes);
