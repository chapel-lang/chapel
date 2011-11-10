var outfile = open("subdir/out.dat", mode.w).writer();

outfile.writeln("Hello, world!");
outfile.close();
