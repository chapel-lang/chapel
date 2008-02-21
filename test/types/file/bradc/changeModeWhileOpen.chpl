var f = new file(filename = "f.out", mode = FileAccessMode.write);

f.open();
f.mode = FileAccessMode.read;
f.close();

writeln("Should never get here!\n");
