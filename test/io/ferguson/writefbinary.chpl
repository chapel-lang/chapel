var u8:uint(64) = 0x1020304050607080;
var u4:uint(32) = 0x01020304;
var u2:uint(16) = 0x1122;
var u1:uint(8) = 0x7F;

var i8:int(64) = 0x1020304050607080;
var i4:int(32) = 0x01020304;
var i2:int(16) = 0x1122;
var i1:int(8) = 0x7F;


writef("%|1i %|1i", i1, i1);
writef("%<1i %>1i", i1, i1);
writef("%<2i %>2i", i2, i2);
writef("%<4i %>4i", i4, i4);
writef("%<8i %>8i", i8, i8);

writef("%|1u %|1u", u1, u1);
writef("%<1u %>1u", u1, u1);
writef("%<2u %>2u", u2, u2);
writef("%<4u %>4u", u4, u4);
writef("%<8u %>8u", u8, u8);


var test = "test";

writef("A");
writef("%>4s", test);
writef("%>*s", 4, test);
writef("B");
writef("%>1S %>2S %>4S %>8S %>vS", test, test, test, test, test);
writef("C");
writef("%|1S %|vS", test, test);
writef("D");
writef("%>0S", test);
writef("%>*S", 1, test);


