use IO;

// readAll can pick up where other byte-reader methods left off:
var ch = openReader("./jab.txt");
const line1 = ch.readLine(bytes);
const line2 = ch.readLine(bytes);
const remaining = ch.readAll(bytes);
ch.close();

// can correctly reproduce the whole file:
writeln(line1 + line2 + remaining);
