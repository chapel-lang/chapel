use IO, Regex;

// regex to read at least two '|'
const bars = new regex("\\|{2,}"),
      ncp = "👉".numCodepoints;

// try to match when a separator isn't present
var r = openReader("maxSizeStringInput.txt");
try {
    writeln(r.readThrough(bars, maxSize=6*ncp));
} catch e {
    writeln(e);
    // ensure pointer was left at its starting position
    writeln(r.readAll());
}
r.close();

// try to match when a *full* separator isn't present
r = openReader("maxSizeStringInput.txt");
try {
    writeln(r.readThrough(bars, maxSize=6*ncp+1));
} catch e {
    writeln(e);
    // ensure pointer was left at its starting position
    writeln(r.readAll());
}
r.close();

// separator is present - don't strip
writeln(openReader("maxSizeStringInput.txt").readThrough(bars, maxSize=6*ncp+3));
writeln(openReader("maxSizeStringInput.txt").readThrough(bars, maxSize=6*ncp+6));

// separator is present - do strip
writeln(openReader("maxSizeStringInput.txt").readThrough(bars, maxSize=6*ncp+3, true));
writeln(openReader("maxSizeStringInput.txt").readThrough(bars, maxSize=6*ncp+6, true));
