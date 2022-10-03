use IO;

// the entire channel is read, and the number of code-points is counted correctly:
var ch = openreader("./jab.txt");
var s = "yep\n";
const num_cp = ch.readAll(s);

writeln(s);
writeln(num_cp);

// nothing remaining to read in the channel:
const s2 = ch.readLine(string);
writeln(s2.isEmpty());

ch.close();
