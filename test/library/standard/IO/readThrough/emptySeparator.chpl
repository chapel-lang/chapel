use IO;

const r = openReader("maxSizeInput.txt");
try {
    r.readThrough("");
} catch e {
    writeln(e);
}
write(r.readAll());
