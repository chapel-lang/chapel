use CSV, IO;

config const filename = "strings.csv";

var reader = openReader(filename, locking=false);
var r = new CSVIO(reader, sep=" ", hasHeader=true);

var A = r.read(string);

writeln(A.domain);
writeln(A);
