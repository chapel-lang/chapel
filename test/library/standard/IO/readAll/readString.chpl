use IO;

var ch = openreader("./jab.txt");
var s = "yep\n";
const num_cp = ch.readAll(s);

writeln(s);
writeln(num_cp);

const s2 = ch.readLine(string);
writeln(s2.isEmpty());

ch.close();
