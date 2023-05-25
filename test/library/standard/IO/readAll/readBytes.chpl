use IO;

// the entire channel is read, and the number of bytes is counted correctly:
var ch = openReader("./jab.txt");
var b = b"yep\n";
const num_b = ch.readAll(b);

writeln(b);
writeln(num_b);

writeln(ch.readAll(b));

ch.close();
