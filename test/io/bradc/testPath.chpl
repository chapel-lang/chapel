var outfile = new file("out.dat", FileAccessMode.write, "subdir");

outfile.open();
outfile.writeln("Hello, world!");
outfile.close();
