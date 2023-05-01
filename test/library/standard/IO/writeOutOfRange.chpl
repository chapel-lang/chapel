use IO;

var filename = "outOfRange.txt";
var f = open(filename, ioMode.cw);

var writeCh = f.writer(region=0..3);
try {
    writeCh.writeln("blah", "blah", "blah");
} catch e : EofError {
    writeln("caught correct error [ ", e, " ]");
} catch e {
    writeln("caught wrong error type [ ", e, " ]");
}
writeCh.close();

writeCh = f.writer(region=0..3);
try {
    writeCh.writeln("blah, blah, blah");
} catch e : UnexpectedEofError {
    writeln("caught correct error [ ", e, " ]");
} catch e {
    writeln("caught wrong error type [ ", e, " ]");
}
writeCh.close();

writeCh = f.writer(region=4..7);
try {
    writeCh.write("blah", "blah", "blah");
} catch e : EofError {
    writeln("caught correct error [ ", e, " ]");
} catch e {
    writeln("caught wrong error type [ ", e, " ]");
}
writeCh.close();

writeCh = f.writer(region=0..3);
try {
    writeCh.write("blah, blah, blah");
} catch e : UnexpectedEofError {
    writeln("caught correct error [ ", e, " ]");
} catch e {
    writeln("caught wrong error type [ ", e, " ]");
}
writeCh.close();

writeCh = f.writer(region=8..);
writeCh.writeln();
writeCh.close();

f.close();
