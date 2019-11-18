use IO;

var infile = open("readTuple.dat", iomode.r);
var reader = infile.reader();

var (a, b, c) = reader.read((int, int, int));
var (x, y, z) = reader.read((real, string, bool));
var t1 = reader.read((int, int, int));
var t2 = reader.read((real, string, bool));

writeln("(a, b, c) is: ", (a, b, c));
writeln("(x, y, z) is: ", (x, y, z));
writeln("t1 is: ", t1);
writeln("t2 is: ", t2);
