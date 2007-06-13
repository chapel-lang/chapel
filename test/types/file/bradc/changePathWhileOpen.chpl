var f = file(filename = "f.out", mode = "w");

f.open();
f.path = "./subdir";
f.close();

writeln("Should never get here!\n");
