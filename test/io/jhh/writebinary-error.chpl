use IO;

var u4:uint(32) = 0x01020304;

try {
    stdout.writeBinary(u4);
} catch e {
    writeln("Caught exception: ", e);
}