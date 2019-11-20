use CSV, IO;

config const filename = "strings.csv";

var reader = openreader(filename);
var r = new CSVIO(reader, sep=" ");

var A = r.read(string);

writeln(A.domain);
writeln(A);
