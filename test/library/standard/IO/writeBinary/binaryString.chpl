use IO;

var w = open("./bs.bin", iomode.cw).writer();
var d : string = "some very interesting output data " + codepointToString(0x24) + "\n";

w.writeBinary(d);

try {
    w.writeBinary(d, 1000);
} catch e: IllegalArgumentError {
    writeln("caught ", e);
} catch e {
    writeln("threw wrong error: ", e);
}
