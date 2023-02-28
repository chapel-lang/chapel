use IO;
var r;

// ---------- string ----------

// separator isn't present in the first 6 codepoints (should throw)
r = openreader("maxSizeInput.txt");
try {
    r.readThrough("x", maxSize=6);
} catch e {
    writeln(e);
    // ensure pointer was left at its starting position
    writeln(r.readAll());
}
r.close();

// separator is present
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=7));
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=8));
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=7, true));
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=8, true));

// ---------- bytes----------

// separator isn't present in the first 6 bytes (should throw)
r = openreader("maxSizeInput.txt");
try {
    r.readThrough(b"x", maxSize=6);
} catch e {
    writeln("\n", e);
    // ensure pointer was left at its starting position
    writeln(r.readAll());
}
r.close();

// separator is present
writeln(openreader("maxSizeInput.txt").readThrough(b"x", maxSize=7));
writeln(openreader("maxSizeInput.txt").readThrough(b"x", maxSize=8));
writeln(openreader("maxSizeInput.txt").readThrough(b"x", maxSize=7, true));
writeln(openreader("maxSizeInput.txt").readThrough(b"x", maxSize=8, true));
