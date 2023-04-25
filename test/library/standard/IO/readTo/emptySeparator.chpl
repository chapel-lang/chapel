use IO;

const r = openReader("pattern.txt");
try {
    r.readTo("");
} catch e {
    writeln(e);
}
write(r.readAll());
