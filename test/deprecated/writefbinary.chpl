config type t = string;

var u1:uint(8) = 0x7F;
var i1:int(8) = 0x7F;

writef("%|1i %|1i":t, i1, i1);
writef("%|1u %|1u":t, u1, u1);

var test = "test";
writef("%|1S %|vS":t, test, test);

var testBytes = "test":bytes;
writef("%|1S %|vS":t, testBytes, testBytes);
