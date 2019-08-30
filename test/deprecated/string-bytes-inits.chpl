var s1 = new string("some chapel string");
var s2 = new string(c"some C string");
var s3 = new string((c"some C string"):c_ptr(uint(8)), length=13, size=14);

var b1 = new bytes(b"some bytes");
var b2 = new bytes(c"some other C string", length=16);
var b3 = new bytes((c"some other C string"):c_ptr(uint(8)), length=16, size=20);
