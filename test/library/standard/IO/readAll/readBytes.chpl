use IO;

var ch = openreader("./jab.txt");
var b = b"yep\n";
const num_b = ch.readAll(b);

writeln(b);
writeln(num_b);

const s = ch.readLine(string);
writeln(s.isEmpty());

ch.close();
