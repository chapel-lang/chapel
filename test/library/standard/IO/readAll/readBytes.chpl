use IO;

// the entire channel is read, and the number of bytes is counted correctly:
var ch = openreader("./jab.txt");
var b = b"yep\n";
const num_b = ch.readAll(b);

writeln(b);
writeln(num_b);

// nothing remaining to read in the channel:
const s = ch.readLine(string);
writeln(s.isEmpty());

ch.close();
