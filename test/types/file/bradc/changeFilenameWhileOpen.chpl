var f = new file(filename = "f.out", mode = FileAccessMode.write);

f.open();
f.filename = "g.out";
f.close();

writeln("Should never get here!\n");
