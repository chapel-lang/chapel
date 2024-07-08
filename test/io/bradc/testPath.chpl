use IO;

var outfile = open("subdir/out.dat", ioMode.cw).writer();

outfile.writeln("Hello, world!");
outfile.close();
