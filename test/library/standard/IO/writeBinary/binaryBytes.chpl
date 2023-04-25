use IO;

var w = open("./bb.bin", ioMode.cw).writer();
var d : bytes = b"some very interesting output data \xF0\x9F\x91\x8D\n";

// test normal behavior
w.writeBinary(d);
w.writeBinary(d, d.size - 5);
w.writeBinary(b"\xf0\x9f\xa4\x8c\n");

// test error handling for bad arguments
try {
    w.writeBinary(d, 1000);
} catch e: IllegalArgumentError {
    writeln("caught ", e);
} catch e {
    writeln("threw wrong error: ", e);
}
