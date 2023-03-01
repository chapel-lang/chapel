use IO, Regex;

// regex to read at least two '|'
const bars = compile("\\|{2,}"),
      nb = "👉".numBytes;

// try to match when a separator isn't present
var r = openReader("maxSizeStringInput.txt");
try {
    writeln(r.readThrough(bars, maxSize=6*nb));
} catch e {
    writeln(e);
    // ensure pointer was left at its starting position
    writeln(r.readAll());
}
r.close();

// try to match when a *full* separator isn't present
r = openReader("maxSizeStringInput.txt");
try {
    writeln(r.readThrough(bars, maxSize=6*nb+1));
} catch e {
    writeln(e);
    // ensure pointer was left at its starting position
    writeln(r.readAll());
}
r.close();

// separator is present - don't strip
writeln(openReader("maxSizeStringInput.txt").readThrough(bars, maxSize=6*nb+3));
writeln(openReader("maxSizeStringInput.txt").readThrough(bars, maxSize=6*nb+3));
writeln(openReader("maxSizeStringInput.txt").readThrough(bars, maxSize=6*nb+6));

// separator is present - do strip
writeln(openReader("maxSizeStringInput.txt").readThrough(bars, maxSize=6*nb+3, true));
writeln(openReader("maxSizeStringInput.txt").readThrough(bars, maxSize=6*nb+3, true));
writeln(openReader("maxSizeStringInput.txt").readThrough(bars, maxSize=6*nb+6, true));
