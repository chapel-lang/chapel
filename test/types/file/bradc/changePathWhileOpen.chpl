var f = new file(filename = "f.out", mode = FileAccessMode.write);

f.open();
f.path = "./subdir";
f.close();

writeln("Should never get here!\n");
