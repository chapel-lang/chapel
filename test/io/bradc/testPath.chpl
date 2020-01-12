use IO;

var outfile = open("subdir/out.dat", iomode.cw).writer();

outfile.writeln("Hello, world!");
outfile.close();
