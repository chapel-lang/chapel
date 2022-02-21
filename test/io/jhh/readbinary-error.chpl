use IO;

var u4:uint(32);
try {
    var rv = stdin.readBinary(u4);
} catch e {
    writeln("Caught exception: ", e);
}
