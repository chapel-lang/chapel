use IO;

var w = open("./bb.bin", iomode.cw).writer();
var d : bytes = b"some very interesting output data \xF0\x9F\x91\x8D\n";

// test normal behavior
w.writeBinary(d);

// test error handling for bad arguments
try {
    w.writeBinary(d, 1000);
} catch e: IllegalArgumentError {
    writeln("caught ", e);
} catch e {
    writeln("threw wrong error: ", e);
}
