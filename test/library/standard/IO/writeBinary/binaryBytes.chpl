use IO;

var w = open("./bb.bin", iomode.cw).writer();
var d : bytes = b"some very interesting output data \xF0\x9F\x91\x8D\n";

w.writeBinary(d);

try {
    w.writeBinary(d, 1000);
} catch e: IllegalArgumentError {
    writeln("caught ", e);
} catch e {
    writeln("threw wrong error: ", e);
}
