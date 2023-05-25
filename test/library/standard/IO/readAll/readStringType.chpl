use IO;

var ch = openReader("./jab.txt");
const s = ch.readAll(string);

writeln(s);
writeln(s.type == string);

// nothing remaining to read in the channel:
try {
    ch.readAll(bytes);
} catch e {
    writeln(e);
}

ch.close();
