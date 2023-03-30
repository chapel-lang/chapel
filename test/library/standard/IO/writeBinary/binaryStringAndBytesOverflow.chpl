use IO;

proc testOverflow(toWrite) {
    var f = open("./sbOverflow.bin", ioMode.cw);
    var w = f.writer(region=0..5);

    // try writing a string/bytes that doesn't fit in the file's specified region
    try {
        w.writeBinary(toWrite);
    } catch e: UnexpectedEofError {
        writeln("caught: ", e);
    } catch e {
        writeln("Threw wrong error: ", e);
    }

    // try writing at EOF
    try {
    w.writeBinary(toWrite[0..1]);
    } catch e: EofError {
        writeln("caught: ", e);
    } catch e {
        writeln("Threw wrong error: ", e);
    }
}

testOverflow("abcdefghijk");
testOverflow(b"abcdefghijk");
