use IO;

// the entire channel is read, and the number of code-points is counted correctly:
var ch = openReader("./jab.txt");
var s = "yep\n";
const num_cp = ch.readAll(s);

writeln(s);
writeln(num_cp);

writeln(ch.readAll(s));

ch.close();
