use IO;

const r = openReader("pattern.txt", locking=false);
try {
    r.readTo("");
} catch e {
    writeln(e);
}
write(r.readAll());
