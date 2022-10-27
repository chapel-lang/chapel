use IO;

var ch = openreader("./jab.txt");
const b = ch.readAll(bytes);
ch.close();

writeln(b);
writeln(b.type == bytes);
