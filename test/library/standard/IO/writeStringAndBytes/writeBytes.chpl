use IO;

var w = openwriter("bfile.txt");
var b = b"negates itself when prepended to itself \xE2\xAC\x85\xEf\xB8\x8F negates itself when prepended to itself";

// default size:
w.writeBytes(b);
w.writeln();

// valid size argument:
w.writeBytes(b, 14);
w.writeln();

// invalid size argument:
try {
    w.writeBytes(b, 100);
} catch e: IllegalArgumentError {
    writeln("caught correct error [ ", e, " ]");
} catch e {
    writeln("caught wrong error [ ", e, " ]");
}
