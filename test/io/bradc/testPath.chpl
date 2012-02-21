var outfile = open("subdir/out.dat", iomode.w).writer();

outfile.writeln("Hello, world!");
outfile.close();
