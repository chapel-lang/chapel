use IO;
var r;

// ---------- string ----------

r = openreader("maxSizeInput.txt");
try {
    r.readThrough("x", maxSize=6);
} catch e {
    writeln(e);
    writeln(r.readAll());
}
r.close();
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=7));
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=8));
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=7, true));
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=8, true));

// ---------- bytes----------
r = openreader("maxSizeInput.txt");
try {
    r.readThrough(b"x", maxSize=6);
} catch e {
    writeln(e);
    writeln(r.readAll());
}
r.close();
writeln(openreader("maxSizeInput.txt").readThrough(b"x", maxSize=7));
writeln(openreader("maxSizeInput.txt").readThrough(b"x", maxSize=8));
writeln(openreader("maxSizeInput.txt").readThrough(b"x", maxSize=7, true));
writeln(openreader("maxSizeInput.txt").readThrough(b"x", maxSize=8, true));
