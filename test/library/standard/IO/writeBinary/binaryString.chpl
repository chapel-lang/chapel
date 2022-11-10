use IO;

var w = open("./bs.bin", iomode.cw).writer();
var d: string = "some very interesting output data 👍\n";

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

var myBytes = b"Illegal \xff sequence";  // \xff is non UTF-8
var myEscapedString = myBytes.decode(policy=decodePolicy.escape);
try {
    w.writeBinary(myEscapedString);
} catch e {
    writeln(e);
}
