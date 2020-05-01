use IO;

config var inputfile = "test.dat";

var infile = open(inputfile, iomode.r).reader();

var n1, n2: int;

var s = infile.readln(string);
n1 = infile.readln(int);
infile.readln(n2);

writeln(s);
writeln((n1,n2));

