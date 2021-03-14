use IO;

var b: bytes;
readf("%|4s", b);
writef("%|4s", b);
var len = 4;
readf("%|*s", len, b);
writef("%|*s", len, b);
