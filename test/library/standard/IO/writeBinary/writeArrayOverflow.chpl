use IO;

var f = open("./arrayOverflow.bin", ioMode.cw);
var w = f.writer(region=0..5);

// try writing an array that doesn't fit in the file's specified region
try {
    w.writeBinary([0:uint(8), 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
} catch e : UnexpectedEofError {
    writeln("caught: ", e);
} catch e {
    writeln("Threw wrong error: ", e);
}

// try writing at EOF
try {
    w.writeBinary([1:uint(8), ]);
} catch e: EofError {
    writeln("caught: ", e);
} catch e {
    writeln("Threw wrong error: ", e);
}
