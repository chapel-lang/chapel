use IO;

// test a format with a non UTF8 char in it
var formatBytes = b"\xff %i\n".format(10);
writef("%|5s", formatBytes);

// test writing an array into bytes (with 0xFF being non UTF8)
var arr = [0:uint(8), 255:uint(8)];
writef(b"%|t", arr);
