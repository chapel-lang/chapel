use IO;

var infile = open("readWholeArr.dat", ioMode.r);
var reader = infile.reader(locking=false);

var A: [1..3] real;

reader.read(A);

writeln("A is: ", A);
