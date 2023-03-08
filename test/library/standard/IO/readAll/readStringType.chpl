use IO;

var ch = openReader("./jab.txt");
const s = ch.readAll(string);
ch.close();

writeln(s);
writeln(s.type == string);
