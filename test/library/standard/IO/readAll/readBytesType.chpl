use IO;

var ch = openReader("./jab.txt");
const b = ch.readAll(bytes);
ch.close();

writeln(b);
writeln(b.type == bytes);
