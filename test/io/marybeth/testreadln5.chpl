config var inputfile = "test.dat";

var infile = new file(inputfile, path='./', mode=FileAccessMode.read);

var s = infile.readln(string);

writeln(s);

