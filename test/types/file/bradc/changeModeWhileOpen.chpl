var f = new file(filename = "f.out", mode = "w");

f.open();
f.mode = "r";
f.close();

writeln("Should never get here!\n");
