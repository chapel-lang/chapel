config var inputfile = "test.dat";

var infile = new file(inputfile, path='./', mode='r');

var s = infile.readln(string);

writeln(s);

