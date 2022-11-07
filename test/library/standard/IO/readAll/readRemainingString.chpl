use IO;

// readAll can pick up where other string-reader methods left off:
var ch = openreader("./jab.txt");
const line1 = ch.readLine(string);
const line2 = ch.readLine(string);
const remaining = ch.readAll(string);
ch.close();

// can correctly reproduce the whole file:
writeln(line1 + line2 + remaining);
