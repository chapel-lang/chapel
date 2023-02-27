use IO, Regex;

// regex to read at least two '|'
const bars = compile(b"\\|{2,}");

// try to match when a separator isn't present
var r = openreader("maxSizeBytesInput.txt");
try {
    writeln(r.readThrough(bars, maxSize=6));
} catch e {
    writeln(e);
    writeln(r.readAll());
}
r.close();

// try to match when a full separator isn't present
r = openreader("maxSizeBytesInput.txt");
try {
    writeln(r.readThrough(bars, maxSize=7));
} catch e {
    writeln(e);
    writeln(r.readAll());
}
r.close();

// separator is present - don't strip
writeln(openreader("maxSizeBytesInput.txt").readThrough(bars, maxSize=8));
writeln(openreader("maxSizeBytesInput.txt").readThrough(bars, maxSize=9));
writeln(openreader("maxSizeBytesInput.txt").readThrough(bars, maxSize=13));

// separator is present - do strip
writeln(openreader("maxSizeBytesInput.txt").readThrough(bars, maxSize=8, true));
writeln(openreader("maxSizeBytesInput.txt").readThrough(bars, maxSize=9, true));
writeln(openreader("maxSizeBytesInput.txt").readThrough(bars, maxSize=13, true));
