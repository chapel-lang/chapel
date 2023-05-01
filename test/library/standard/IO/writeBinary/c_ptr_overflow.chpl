use IO, CTypes;

proc testCPtrOverflow(toWrite) {
    var f = open("./cptrOverflow.bin", ioMode.cw);
    var w = f.writer(region=0..5);

    // try writing an array that doesn't fit in the file's specified region
    try {
        w.writeBinary(toWrite, 10);
    } catch e : UnexpectedEofError {
        writeln("caught: ", e);
    } catch e {
        writeln("Threw wrong error: ", e);
    }

    // try writing at EOF
    try {
        w.writeBinary(toWrite, 1);
    } catch e: EofError {
        writeln("caught: ", e);
    } catch e {
        writeln("Threw wrong error: ", e);
    }
}

var a = [1,2,3,4,5,6,7,8,9,10]:uint(8);

testCPtrOverflow(c_ptrTo(a));
testCPtrOverflow(c_ptrTo(a): c_void_ptr);
