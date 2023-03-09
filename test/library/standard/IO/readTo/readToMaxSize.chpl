use IO;
var r;

const fname = "../readThrough/maxSizeInput.txt";

// ---------- string ----------

// separator isn't present in the first 6 codepoints (should throw)
r = openReader(fname);
try {
    r.readTo("x", maxSize=6);
} catch e {
    writeln(e);
    // ensure pointer was left at its starting position
    writeln(r.readAll());
}

// separator is present
writeln(openReader(fname).readTo("x", maxSize=7));
writeln(openReader(fname).readTo("x", maxSize=8));

// ---------- bytes----------

// separator isn't present in the first 6 bytes (should throw)
r = openReader(fname);
try {
    r.readTo(b"x", maxSize=6);
} catch e {
    writeln("\n", e);
    // ensure pointer was left at its starting position
    writeln(r.readAll());
}

// separator is present
writeln(openReader(fname).readTo(b"x", maxSize=7));
writeln(openReader(fname).readTo(b"x", maxSize=8));
