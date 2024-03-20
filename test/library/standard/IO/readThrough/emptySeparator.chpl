use IO;

const r = openReader("maxSizeInput.txt", locking=false);
try {
    r.readThrough("");
} catch e {
    writeln(e);
}
write(r.readAll());
