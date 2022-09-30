use IO;

var a : [0..<1040] uint(8) = 0;
var ch = openreader("./jab.txt");
const num_b = ch.readAll(a);
ch.close();

writeln(num_b == 1032);
writeln(&& reduce (a[1033..] == 0));
