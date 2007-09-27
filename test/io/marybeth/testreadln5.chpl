config var inputfile = "test.dat";

var infile = file(inputfile, path='./', mode='r');

var s = infile.readln(string);

writeln(s);

