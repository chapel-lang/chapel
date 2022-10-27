use IO;

var ch = openreader("./jab.txt");
const s = ch.readAll(string);
ch.close();

writeln(s);
writeln(s.type == string);
