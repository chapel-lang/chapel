use IO;

var ch = openReader("./jab.txt");
const b = ch.readAll(bytes);
ch.close();

writeln(b);
writeln(b.type == bytes);

// nothing remaining to read in the channel:
try {
    ch.readAll(bytes);
} catch e {
    writeln(e);
}
