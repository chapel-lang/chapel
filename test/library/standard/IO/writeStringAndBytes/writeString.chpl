use IO;

var w = openWriter("sfile.txt", locking=false);
var s = "negates itself when prepended to itself ⬅️ negates itself when prepended to itself";

// default size:
w.writeString(s);
w.writeln();

// valid size argument:
w.writeString(s, 14);
w.writeln();

// invalid size argument:
try {
    w.writeString(s, 100);
} catch e: IllegalArgumentError {
    writeln("caught correct error [ ", e, " ]");
} catch e {
    writeln("caught wrong error [ ", e, " ]");
}
