use IO;

const n = 5;

var f = open("./test.bin", iomode.cw);
var w = f.writer(region=0..5);

try {
    w.writeBinary([0:uint(8), 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
} catch e : UnexpectedEofError {
    writeln("caught: ", e);
} catch e {
    writeln("Threw wrong error: ", e);
}
