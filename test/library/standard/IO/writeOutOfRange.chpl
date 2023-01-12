use IO;

var filename = "outOfRange.txt";
var f = open(filename, iomode.cw);

var writeCh = f.writer(region=0..3);
try {
    writeCh.writeln("blah blah blah");
} catch e : EofError {
    writeln("caught correct error [ ", e, " ]");
} catch e {
    writeln("caught wrong error type [ ", e, " ]");
}
writeCh.close();

writeCh = f.writer(region=0..3);
try {
    writeCh.write("blah blah blah");
} catch e : EofError {
    writeln("caught correct error [ ", e, " ]");
} catch e {
    writeln("caught wrong error type [ ", e, " ]");
}
writeCh.close();

f.close();
